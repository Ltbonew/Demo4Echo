#include "../inc/Application.h"
#include "../inc/user_log.h"
#include "../third_party/snowboy/include/snowboy-detect-c-wrapper.h"

Application::Application(const std::string& address, int port, const std::string& token, const std::string& deviceId, int protocolVersion, AudioProcess& audio_processor)
    : ws_client_(address, port, token, deviceId, protocolVersion),
      protocolVersion_(protocolVersion),
      client_state_(static_cast<int>(AppState::idle)),
      audio_processor_(audio_processor) {

        // 设置接收到消息的回调函数
        ws_client_.SetMessageCallback([this](const std::string& message, bool is_binary) {
            ws_msg_callback(message, is_binary);
        });
        ws_client_.SetCloseCallback([this]() {
            // 断开连接时的回调
            
        });
}

void Application::ws_msg_callback(const std::string& message, bool is_binary) {

    if(!is_binary) {
        // 接收到消息时的回调
        messageQueue_.Enqueue(message);
    } else {
        // 接收到二进制数据时的回调
        BinProtocolInfo protocol_info;
        std::vector<uint8_t> opus_data;
        std::vector<int16_t> pcm_data;

        // 解包二进制数据
        if(audio_processor_.UnpackBinFrame(reinterpret_cast<const uint8_t*>(message.data()), message.size(), protocol_info, opus_data)) {
            // 检查版本和类型是否符合预期
            if(protocol_info.version == protocolVersion_ && protocol_info.type == 0) {
                // 将解码后的Opus数据放入队列供播放器使用
                audio_processor_.decode(opus_data.data(), opus_data.size(), pcm_data);
                audio_processor_.addFrameToPlaybackQueue(pcm_data);
            } else {
                USER_LOG_WARN("Received frame with unexpected version or type");
            }
        } else {
            USER_LOG_WARN("Failed to unpack binary frame");
        }
    }
}

// 处理接websocket收到的消息并返回事件号
Application::AppEvent_t_ Application::handle_message(const std::string& message) {
    Json::Value root;
    Json::Reader reader;
    // 解析 JSON 字符串
    bool parsingSuccessful = reader.parse(message, root);
    if (!parsingSuccessful) {
        USER_LOG_WARN("Error parsing message: %s", reader.getFormattedErrorMessages().c_str());
        return static_cast<int>(AppEvent::fault);
    }
    // 获取 JSON 对象中的值
    const Json::Value type = root["type"];
    if (type.isString()) {
        std::string typeStr = type.asString();
        if (typeStr == "vad") {
            return handle_vad_message(root);
        } else if (typeStr == "asr") {
            return handle_asr_message(root);
        } else if (typeStr == "tts") {
            return handle_tts_message(root);
        } else if (typeStr == "error") {
            USER_LOG_ERROR("server erro msg: %s", message.c_str());
            return static_cast<int>(AppEvent::fault);
        }
    }
    USER_LOG_WARN("not event message type: %s", message.c_str());
    return -1;
}

// 处理 VAD 消息
Application::AppEvent_t_ Application::handle_vad_message(const Json::Value& root) {
    const Json::Value state = root["state"];
    if (state.isString()) {
        std::string stateStr = state.asString();
        if (stateStr == "no_speech") {
            return static_cast<int>(AppEvent::vad_no_speech);
        } else if (stateStr == "end" || stateStr == "too_long") {
            return static_cast<int>(AppEvent::vad_end);
        } 
    }
    return -1;
}

// 处理 ASR 消息
Application::AppEvent_t_ Application::handle_asr_message(const Json::Value& root) {
    const Json::Value text = root["text"];
    if (text.isString()) {
        asr_text_ = text.asString();
        USER_LOG_INFO("Received ASR text: %s", asr_text_.c_str());
    } else {
        USER_LOG_WARN("Invalid ASR text value.");
    }
    return static_cast<int>(AppEvent::asr_received);
}

// 处理 TTS 消息
Application::AppEvent_t_ Application::handle_tts_message(const Json::Value& root) {
    const Json::Value state = root["state"];
    if (state.isString()) {
        std::string stateStr = state.asString();
        if (stateStr == "end") {
            USER_LOG_INFO("Received TTS end.");
            tts_completed_ = true;
        }
    }
    const Json::Value conversation = root["conversation"];
    if (conversation.isString()) {
        std::string conversationStr = conversation.asString();
        if (conversationStr == "end") {
            USER_LOG_INFO("Received conversation end.");
            conversation_completed_ = true;
        }
    }
    return -1;
}


void Application::idle_enter() {
    std::string json_message = R"({"type": "state", "state": "idle"})";
    ws_client_.SendText(json_message);
    // clear recorded audio queue
    audio_processor_.clearRecordedAudioQueue();
    // 开启录音
    audio_processor_.startRecording();
    // start state running
    state_running_ = true;
    state_running_thread_ = std::thread([this]() { idleState_run(); });
    USER_LOG_INFO("Into Idle state.");
}

void Application::idleState_run() {
    SnowboyDetect* detector = SnowboyDetectConstructor("third_party/snowboy/resources/common.res",
                                                     "third_party/snowboy/resources/models/echo.pmdl");
    SnowboyDetectSetSensitivity(detector, "0.5");
    SnowboyDetectSetAudioGain(detector, 1);
    SnowboyDetectApplyFrontend(detector, false);
    std::vector<int16_t> data;
    while (state_running_ == true) {
        if(audio_processor_.getRecordedAudio(data)) {
            // 检测唤醒词
            int result = SnowboyDetectRunDetection(detector, data.data(), data.size(), false);
            if (result > 0) {
                // 发生唤醒事件
                USER_LOG_INFO("Wake detected.");
                eventQueue_.Enqueue(static_cast<int>(AppEvent::wake_detected));
                break;
            }
        }
    }
    SnowboyDetectDestructor(detector);
}

void Application::idle_exit() {
    // stop录音
    audio_processor_.stopRecording();
    // stop running
    state_running_ = false;
    state_running_thread_.join();

    // playing waked up sound
    std::string waked_sound_path = "third_party/audio/waked.pcm";
    auto audioQueue = audio_processor_.loadAudioFromFile(waked_sound_path, 40);
    while (!audioQueue.empty()) {
        const auto& frame = audioQueue.front();
        audio_processor_.addFrameToPlaybackQueue(frame);
        audioQueue.pop();
    }
    audio_processor_.startPlaying();
    while(!audio_processor_.playbackQueueIsEmpty()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    // audio_processor_.stopPlaying();

    USER_LOG_INFO("Idle exit.");
}

void Application::listening_enter() {
    std::string json_message = R"({"type": "state", "state": "listening"})";
    ws_client_.SendText(json_message);
    // start录音
    audio_processor_.startRecording();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    // clear recorded audio queue
    audio_processor_.clearRecordedAudioQueue();
    // clear playback audio queue
    audio_processor_.clearPlaybackAudioQueue();
    // running
    state_running_ = true;
    state_running_thread_ = std::thread([this]() { listeningState_run(); });
    USER_LOG_INFO("Into listening state.");
}

void Application::listeningState_run() {
    while (state_running_ == true) {
        std::vector<int16_t> audio_frame;
        if(audio_processor_.recordedQueueIsEmpty() == false) {
            if(audio_processor_.getRecordedAudio(audio_frame)) {
                // 编码
                uint8_t opus_data[1536];
                size_t opus_data_size;
                if (audio_processor_.encode(audio_frame, opus_data, opus_data_size)) {
                    // 打包
                    BinProtocol* packed_frame = audio_processor_.PackBinFrame(opus_data, opus_data_size);
                    if (packed_frame) {
                        // 发送
                        ws_client_.SendBinary(reinterpret_cast<uint8_t*>(packed_frame), sizeof(BinProtocol) + opus_data_size);
                    } else {
                        USER_LOG_WARN("Audio Packing failed");
                    }
                } else {
                    USER_LOG_WARN("Audio Encoding failed");
                }
            }
        }
    }
}

void Application::listening_exit() {
    // stop录音
    audio_processor_.stopRecording();
    // stop running
    state_running_ = false;
    state_running_thread_.join();
    USER_LOG_INFO("Listening exit.");
}

void Application::speaking_enter() {
    std::string json_message = R"({"type": "state", "state": "speaking"})";
    ws_client_.SendText(json_message);
    // start播放
    audio_processor_.startPlaying();
    // running
    state_running_ = true;
    state_running_thread_ = std::thread([this]() { speakingState_run(); });
    USER_LOG_INFO("Into speaking state.");
}

void Application::speakingState_run() {
    USER_LOG_INFO("Speaking state run.");
    while(state_running_ == true) {
        if(tts_completed_ && audio_processor_.playbackQueueIsEmpty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            USER_LOG_INFO("Speaking end.");
            if(conversation_completed_ == false) {
                eventQueue_.Enqueue(static_cast<int>(AppEvent::speaking_end));
            } else {
                eventQueue_.Enqueue(static_cast<int>(AppEvent::conversation_end));
            }
            tts_completed_ = false;
            conversation_completed_ = false;
            break;
        }
    }
}

void Application::speaking_exit() {
    // clear playback audio queue
    audio_processor_.clearPlaybackAudioQueue();
    // stop播放
    // audio_processor_.stopPlaying();
    // stop state running
    state_running_ = false;
    state_running_thread_.join();
    USER_LOG_INFO("Speaking exit.");
}

void Application::Run() {

    ws_client_.Run(); // 会开一个thread
    ws_client_.Connect();
    // 必须等待连接建立
    while(!ws_client_.IsConnected()) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        ws_client_.Connect();
    }
    
    if (ws_client_.IsConnected()) {

        std::string json_message = 
        R"({
            "type": "hello",
            "audio_params": {
                "format": "opus",
                "sample_rate": )" + std::to_string(audio_processor_.get_sample_rate()) + R"(,
                "channels": )" + std::to_string(audio_processor_.get_channels()) + R"(,
                "frame_duration": )" + std::to_string(audio_processor_.get_frame_duration()) + R"(
            }
        })";

        ws_client_.SendText(json_message);
    }

    std::thread ws_msg_thread([this]() {
        // websocket收到的json处理
        while(true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(auto message_opt = messageQueue_.Dequeue(); message_opt) {
                std::string message = message_opt.value();
                // 检查是否为 "null" 或者是空字符串
                if (message == "null" || message.empty()) {
                    continue; // 跳过本次循环的后续操作
                }
                // 处理信息转化为事件
                int event = handle_message(message);
                // 发送事件到事件队列
                if(event!=-1) {
                    eventQueue_.Enqueue(event);
                }
            }
        }
    });

    std::thread state_trans_thread([this]() {
        // 添加状态
        client_state_.RegisterState(static_cast<int>(AppState::idle), [this]() {idle_enter();}, [this]() {idle_exit();});
        client_state_.RegisterState(static_cast<int>(AppState::listening), [this]() {listening_enter(); }, [this]() {listening_exit(); });
        client_state_.RegisterState(static_cast<int>(AppState::speaking), [this]() {speaking_enter(); }, [this]() {speaking_exit(); });
        
        // 添加状态切换
        client_state_.RegisterTransition(static_cast<int>(AppState::idle), static_cast<int>(AppEvent::wake_detected), static_cast<int>(AppState::listening));
        client_state_.RegisterTransition(static_cast<int>(AppState::listening), static_cast<int>(AppEvent::vad_no_speech), static_cast<int>(AppState::idle));
        client_state_.RegisterTransition(static_cast<int>(AppState::listening), static_cast<int>(AppEvent::vad_end), static_cast<int>(AppState::speaking));
        client_state_.RegisterTransition(static_cast<int>(AppState::speaking), static_cast<int>(AppEvent::speaking_end), static_cast<int>(AppState::listening));
        client_state_.RegisterTransition(static_cast<int>(AppState::speaking), static_cast<int>(AppEvent::conversation_end), static_cast<int>(AppState::idle));
        client_state_.RegisterTransition(-1, static_cast<int>(AppEvent::fault), static_cast<int>(AppState::idle));
        // 初始化
        client_state_.Initialize();

        // 主要执行state事件处理, 状态切换
        while(true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            // 事件queue处理
            if (auto event_opt = eventQueue_.Dequeue(); event_opt) {
                client_state_.HandleEvent(event_opt.value());
            }
        }
    });

    // 等待 处理websocket msg的 线程结束
    ws_msg_thread.join();
    // 等待 state 切换事件线程结束
    state_trans_thread.join();
  
}