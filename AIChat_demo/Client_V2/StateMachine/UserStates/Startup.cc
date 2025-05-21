#include "Startup.h"
#include "../../Utils/user_log.h"
#include "../../Application/Application.h"
#include "../../Events/AppEvents.h"

void StartupState::Enter(Application* app) {
    USER_LOG_INFO("Into startup state.");
    app->ws_client_.Run(); // 会开一个thread
    app->ws_client_.Connect();
    // 等待连接建立, 尝试3次
    int try_count = 1;
    while(!app->ws_client_.IsConnected() && try_count && !app->get_threads_stop_sig()) {
        try_count--;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        USER_LOG_INFO("Try to connect to server.");
        app->ws_client_.Connect();
    }
    
    if (app->ws_client_.IsConnected()) {
        std::string json_message = 
        R"({
            "type": "hello",
            "api_key": ")" + app->get_aliyun_api_key() + R"(",
            "audio_params": {
                "format": "opus",
                "sample_rate": )" + std::to_string(app->audio_processor_.get_sample_rate()) + R"(,
                "channels": )" + std::to_string(app->audio_processor_.get_channels()) + R"(,
                "frame_duration": )" + std::to_string(app->audio_processor_.get_frame_duration()) + R"(
            }
        })";

        app->ws_client_.SendText(json_message);
        app->eventQueue_.Enqueue(static_cast<int>(AppEvent::startup_done));
    }
    else {
        USER_LOG_ERROR("Startup failed.");
        app->eventQueue_.Enqueue(static_cast<int>(AppEvent::to_stop));
    }
}

void StartupState::Exit(Application* app) {
    USER_LOG_INFO("Startup exit.");
}