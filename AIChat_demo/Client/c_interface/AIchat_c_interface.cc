#include "AIchat_c_interface.h"
#include "../inc/Application.h"

extern "C" {

// 创建并初始化Application对象
void* create_aichat_app(const char* address, int port, const char* token, const char* deviceId, const char* aliyun_api_key, int protocolVersion, int sample_rate, int channels, int frame_duration) {
    auto* app = new Application(std::string(address), port, std::string(token), std::string(deviceId), std::string(aliyun_api_key), protocolVersion, sample_rate, channels, frame_duration);
    return static_cast<void*>(app);
}

// 运行Application对象
void run_aichat_app(void* app_ptr) {
    if (app_ptr) {
        auto* app = static_cast<Application*>(app_ptr);
        app->Run();
    }
}

// 强制停止Application对象
void stop_aichat_app(void* app_ptr) {
    if (app_ptr) {
        auto* app = static_cast<Application*>(app_ptr);
        app->Stop();
    }
}

// 获取当前状态
ChatState get_aichat_app_state(void* app_ptr) {
    if (app_ptr) {
        auto* app = static_cast<Application*>(app_ptr);
        return static_cast<ChatState>(app->getState());
    }
    return ChatState::fault; // 默认返回错误状态
}

// 销毁Application对象
void destroy_aichat_app(void* app_ptr) {
    if (app_ptr) {
        auto* app = static_cast<Application*>(app_ptr);
        delete app;
    }
}

} // extern "C"