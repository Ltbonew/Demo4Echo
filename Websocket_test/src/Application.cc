#include "../inc/Application.h"

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
int Application::handle_message(const std::string& message) {
    Json::Value root;
    Json::Reader reader;
    // 解析 JSON 字符串
    bool parsingSuccessful = reader.parse(message, root);
    if (!parsingSuccessful) {
        ws_client_.Log("Error parsing message: " + reader.getFormattedErrorMessages(), WebSocketClient::LogLevel::ERROR);
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
    ws_client_.Log("not event message type: " + message, WebSocketClient::LogLevel::WARNING);
    return NULL;
}

// 处理 VAD 消息
int Application::handle_vad_message(const Json::Value& root) {
    const Json::Value state = root["state"];
    if (state.isString()) {
        std::string stateStr = state.asString();
        if (stateStr == "no_speech") {
            return static_cast<int>(AppEvent::vad_no_speech);
        } else if (stateStr == "end" || stateStr == "too_long") {
            return static_cast<int>(AppEvent::vad_end);
        } 
    }
}

// 处理 ASR 消息
int Application::handle_asr_message(const Json::Value& root) {
    const Json::Value text = root["text"];
    if (text.isString()) {
        std::string textStr = text.asString();
        ws_client_.Log("Received ASR text: " + textStr);
    } else {
        ws_client_.Log("Invalid ASR text value.", WebSocketClient::LogLevel::ERROR);
    }
    return static_cast<int>(AppEvent::asr_received);
}

void Application::idle_enter() {
    std::string json_message = R"({"type": "state", "state": "idle"})";
    ws_client_.SendText(json_message);
    client_state_.Log("Into Idle state.");
    // 开启录音
    
}

void Application::idle_exit() {
    client_state_.Log("idle exit.");
}

void Application::listening_enter() {
    std::string json_message = R"({"type": "state", "state": "listening"})";
    ws_client_.SendText(json_message);
    client_state_.Log("Into listening state.");
}

void Application::listening_exit() {
    client_state_.Log("listening exit.");
}

void Application::thinking_enter() {
    std::string json_message = R"({"type": "state", "state": "thinking"})";
    ws_client_.SendText(json_message);
    client_state_.Log("Into thinking state.");
}

void Application::thinking_exit() {
    client_state_.Log("thinking exit.");
}

void Application::speaking_enter() {
    std::string json_message = R"({"type": "state", "state": "speaking"})";
    ws_client_.SendText(json_message);
    client_state_.Log("Into speaking state.");
}

void Application::speaking_exit() {
    client_state_.Log("speaking exit.");
}

// void Application::ListeningState() {
//     std::string json_message = R"({"type": "state", "state": "listening"})";
//     ws_client_.SendText(json_message);
//     client_state_.Log("Into Listening state.");

//     AudioProcess audio_processor(sample_rate_, channels_);
//     std::queue<std::vector<int16_t>> audio_queue_ = audio_processor.loadAudioFromFile("../test_audio/out_chock2inmid.pcm", frame_duration_);

//     while (client_state_.GetCurrentState() == "listening") {
//         if(!audio_queue_.empty()) {
//             std::vector<int16_t> pcm_frame = audio_queue_.front();
//             audio_queue_.pop();

//             uint8_t opus_data[1536];
//             size_t opus_data_size;

//             if (audio_processor.encode(pcm_frame, opus_data, opus_data_size)) {
//                 // 打包
//                 BinProtocol* packed_frame = audio_processor.PackBinFrame(opus_data, opus_data_size);

//                 if (packed_frame) {
//                     // 发送
//                     ws_client_.SendBinary(reinterpret_cast<uint8_t*>(packed_frame), sizeof(BinProtocol) + opus_data_size);
//                 } else {
//                     audio_processor.Log("Packing failed", audio_processor.ERROR);
//                 }
//             } else {
//                 audio_processor.Log("Encoding failed", audio_processor.ERROR);
//             }
//         }
//     }
//     client_state_.RequestTransitTo("thinking");
// }

// void Application::ThinkState() {
//     std::string json_message = R"({"type": "state", "state": "thinking"})";
//     ws_client_.SendText(json_message);
//     client_state_.Log("Into thinking state.");

//     while(1);

//     // 模拟思考过程
//     std::this_thread::sleep_for(std::chrono::seconds(2));

//     // 模拟服务器发送text到 LLM 并生成语音
//     std::string llm_response_message = R"({"type": "llm", "text": "这是LLM生成的响应"})";
//     ws_client_.SendText(llm_response_message);

//     client_state_.Log("LLM response received, transitioning to Speaking state."); // 使用 client_state_ 的 Log 方法
//     client_state_.TransitionTo("speaking");
// }

// void Application::SpeakingState() {
//     std::string json_message = R"({"type": "state", "state": "speaking"})";
//     ws_client_.SendText(json_message);
//     client_state_.Log("Into Speaking state.");

//     // 模拟播放语音
//     std::this_thread::sleep_for(std::chrono::seconds(2));

//     client_state_.Log("Speaking finished, transitioning to Idle state."); // 使用 client_state_ 的 Log 方法
//     client_state_.TransitionTo("idle");
// }


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
                // 模拟检测到唤醒词
                std::this_thread::sleep_for(std::chrono::seconds(2));
                // 发生唤醒事件
                client_state_.Log("Wake detected.");
                eventQueue_.Enqueue(static_cast<int>(AppEvent::wake_detected));
                // 关闭录音
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
            if(client_state_.GetCurrentState() == static_cast<int>(AppState::listening)){
                std::this_thread::sleep_for(std::chrono::seconds(1));
                client_state_.Log("listening...");
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