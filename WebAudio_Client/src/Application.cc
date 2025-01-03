#include "../inc/Application.h"
#include "../inc/user_log.h"

Application::Application(const std::string& address, int port, const std::string& token, const std::string& deviceId, const std::string& protocolVersion, int sample_rate, int channels, int frame_duration)
    : ws_client_(address, port, token, deviceId, protocolVersion),
      client_state_(static_cast<int>(AppState::idle)),
      sample_rate_(sample_rate),
      channels_(channels),
      frame_duration_(frame_duration) {

        // 设置接收到消息的回调函数
        ws_client_.SetMessageCallback([this](const std::string& message) {
            // 接收到消息时的回调
            messageQueue_.Enqueue(message);
        });
        ws_client_.SetCloseCallback([this]() {
            // 断开连接时的回调
            
        });
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
        std::string textStr = text.asString();
        USER_LOG_INFO("Received ASR text: %s", textStr.c_str());
    } else {
        USER_LOG_WARN("Invalid ASR text value.");
    }
    return static_cast<int>(AppEvent::asr_received);
}

void Application::idle_enter() {
    std::string json_message = R"({"type": "state", "state": "idle"})";
    ws_client_.SendText(json_message);
    USER_LOG_INFO("Into Idle state.");
    // 开启录音
    
}

void Application::idle_exit() {
    USER_LOG_INFO("Idle exit.");
}

void Application::listening_enter() {
    std::string json_message = R"({"type": "state", "state": "listening"})";
    ws_client_.SendText(json_message);
    USER_LOG_INFO("Into listening state.");
}

void Application::listening_exit() {
    USER_LOG_INFO("Listening exit.");
}

void Application::thinking_enter() {
    std::string json_message = R"({"type": "state", "state": "thinking"})";
    ws_client_.SendText(json_message);
    USER_LOG_INFO("Into thinking state.");
}

void Application::thinking_exit() {
    USER_LOG_INFO("Thinking exit.");
}

void Application::speaking_enter() {
    std::string json_message = R"({"type": "state", "state": "speaking"})";
    ws_client_.SendText(json_message);
    USER_LOG_INFO("Into speaking state.");
}

void Application::speaking_exit() {
    USER_LOG_INFO("Speaking exit.");
}

void Application::idleState_run() {
    // 模拟检测到唤醒词
    std::this_thread::sleep_for(std::chrono::seconds(2));
    // 发生唤醒事件
    USER_LOG_INFO("Wake detected.");
    eventQueue_.Enqueue(static_cast<int>(AppEvent::wake_detected));
    // 关闭录音
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

void Application::listeningState_run() {

    AudioProcess audio_processor(sample_rate_, channels_);
    std::queue<std::vector<int16_t>> audio_queue_ = audio_processor.loadAudioFromFile("../test_audio/out_chock2inmid.pcm", frame_duration_);

    while (client_state_.GetCurrentState() == static_cast<int>(AppState::listening)) {
        if(!audio_queue_.empty()) {
            std::vector<int16_t> pcm_frame = audio_queue_.front();
            audio_queue_.pop();

            uint8_t opus_data[1536];
            size_t opus_data_size;

            if (audio_processor.encode(pcm_frame, opus_data, opus_data_size)) {
                // 打包
                BinProtocol* packed_frame = audio_processor.PackBinFrame(opus_data, opus_data_size);

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

void Application::thinkingState_run() {

    while(1);

}

void Application::speakingState_run() {

    // 模拟播放语音
    std::this_thread::sleep_for(std::chrono::seconds(2));
    USER_LOG_INFO("Speaking finished, transitioning to Idle state.");
}


void Application::Run() {

    ws_client_.Run(); // 会开一个thread
    ws_client_.Connect();
    // 必须等待连接建立
    while(!ws_client_.IsConnected()) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        ws_client_.Connect();
    }
    
    if (ws_client_.IsConnected()) {

        std::string json_message = 
        R"({
            "type": "hello",
            "audio_params": {
                "format": "opus",
                "sample_rate": )" + std::to_string(sample_rate_) + R"(,
                "channels": )" + std::to_string(channels_) + R"(,
                "frame_duration": )" + std::to_string(frame_duration_) + R"(
            }
        })";

        ws_client_.SendText(json_message);
    }

    std::thread ws_msg_thread([this]() {
        // websocket收到的json处理
        while(true) {
            if(auto message_opt = messageQueue_.Dequeue(); message_opt) {
                std::string message = message_opt.value();
                if (!message.empty()) {
                    int event = handle_message(message);
                    // 发送事件到事件队列
                    if(event)
                        eventQueue_.Enqueue(event);
                }
            }
        }
    });

    std::thread state_thread([this]() {
        // 添加状态
        client_state_.RegisterState(static_cast<int>(AppState::idle), [this]() {idle_enter();}, [this]() {idle_exit();});
        client_state_.RegisterState(static_cast<int>(AppState::listening), [this]() {listening_enter(); }, [this]() {listening_exit(); });
        client_state_.RegisterState(static_cast<int>(AppState::thinking), [this]() {thinking_enter(); }, [this]() {thinking_exit(); });
        client_state_.RegisterState(static_cast<int>(AppState::speaking), [this]() {speaking_enter(); }, [this]() {speaking_exit(); });
        
        // 添加状态转移
        client_state_.RegisterTransition(static_cast<int>(AppState::idle), static_cast<int>(AppEvent::wake_detected), static_cast<int>(AppState::listening));
        client_state_.RegisterTransition(static_cast<int>(AppState::listening), static_cast<int>(AppEvent::vad_no_speech), static_cast<int>(AppState::idle));
        client_state_.RegisterTransition(static_cast<int>(AppState::listening), static_cast<int>(AppEvent::vad_end), static_cast<int>(AppState::thinking));
        
        // 初始化
        client_state_.Initialize();

        // 主要执行state事件处理
        while(true) {
            // 事件queue处理
            if (auto event_opt = eventQueue_.Dequeue(); event_opt) {
                client_state_.HandleEvent(event_opt.value());
            }
        }
    });

    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    std::thread AIAudio_thread([this]() {
        while(true) {
            if(client_state_.GetCurrentState() == static_cast<int>(AppState::idle)) {
                idleState_run();
            }
            else if(client_state_.GetCurrentState() == static_cast<int>(AppState::listening)){
                listeningState_run();
            }
            else if(client_state_.GetCurrentState() == static_cast<int>(AppState::thinking)){
                thinkingState_run();
            }
            else if(client_state_.GetCurrentState() == static_cast<int>(AppState::speaking)){
                speakingState_run();
            }
        }
    });

    // 等待 处理websocket msg的 线程结束
    ws_msg_thread.join();
    // 等待 state 线程结束
    state_thread.join();
    // 等待 AIAudio 线程结束
    AIAudio_thread.join();
        
}