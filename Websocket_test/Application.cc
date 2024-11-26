#include "Application.h"

Application::Application(const std::string& address, int port, const std::string& token, const std::string& deviceId, const std::string& protocolVersion, int sample_rate, int channels, int frame_duration)
    : ws_client_(address, port, token, deviceId, protocolVersion),
      client_state_(),
      sample_rate_(sample_rate),
      channels_(channels),
      frame_duration_(frame_duration) {
    // 设置接收到消息的回调函数
    ws_client_.SetMessageCallback([this](const std::string& message) {
        ws_client_.Log("Received message: " + message);
    });

    // 添加状态
    client_state_.AddState("idle", [this]() { IdleState(); });
    client_state_.AddState("listening", [this]() { ListeningState(); });
    client_state_.AddState("think", [this]() { ThinkState(); });
    client_state_.AddState("speaking", [this]() { SpeakingState(); });
}

void Application::Run() {
    // 创建一个线程来连接服务器
    std::thread ws_thread([this]() {
        ws_client_.Connect();
    });

    while (!ws_client_.IsConnected()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ws_client_.Log("Waiting for connection..."); // 使用 client_state_ 的 Log 方法
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

        // 设置初始状态
        client_state_.SetInitialState("idle");

        // 等待 WebSocket 线程结束
        ws_thread.join();
    }
}

void Application::IdleState() {
    std::string json_message = R"({"type": "new_state", "state": "idle"})";
    ws_client_.SendText(json_message);
    client_state_.Log("Into Idle state.");
    // 模拟 KWS 识别到唤醒词
    std::this_thread::sleep_for(std::chrono::seconds(2));
    client_state_.Log("Wakeword detected, transitioning to Listening state."); // 使用 client_state_ 的 Log 方法
    client_state_.TransitionTo("listening");
}

void Application::ListeningState() {
    std::string json_message = R"({"type": "new_state", "state": "listening"})";
    ws_client_.SendText(json_message);
    client_state_.Log("Into Listening state.");
    while(1);
    AudioProcess audio_processor(sample_rate_, channels_);
    std::queue<std::vector<int16_t>> audio_queue_ = audio_processor.loadAudioFromFile("../test_audio/test.pcm", frame_duration_);

    while (!audio_queue_.empty()) {
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
                audio_processor.Log("Packing failed", audio_processor.ERROR);
            }
        } else {
            audio_processor.Log("Encoding failed", audio_processor.ERROR);
        }
    }

    // 模拟 VAD 识别到说话中断, 收到了服务器的Json消息
    client_state_.Log("VAD end detected, transitioning to Think state."); // 使用 client_state_ 的 Log 方法
    client_state_.TransitionTo("think");
}

void Application::ThinkState() {
    std::string json_message = R"({"type": "new_state", "state": "think"})";
    ws_client_.SendText(json_message);
    client_state_.Log("Into Think state.");

    // 模拟思考过程
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 模拟服务器发送text到 LLM 并生成语音
    std::string llm_response_message = R"({"type": "llm", "text": "这是LLM生成的响应"})";
    ws_client_.SendText(llm_response_message);

    client_state_.Log("LLM response received, transitioning to Speaking state."); // 使用 client_state_ 的 Log 方法
    client_state_.TransitionTo("speaking");
}

void Application::SpeakingState() {
    std::string json_message = R"({"type": "new_state", "state": "speaking"})";
    ws_client_.SendText(json_message);
    client_state_.Log("Into Speaking state.");

    // 模拟播放语音
    std::this_thread::sleep_for(std::chrono::seconds(2));

    client_state_.Log("Speaking finished, transitioning to Idle state."); // 使用 client_state_ 的 Log 方法
    client_state_.TransitionTo("idle");
}