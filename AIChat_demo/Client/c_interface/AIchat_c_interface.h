#ifndef AICHAT_C_INTERFACE_H
#define AICHAT_C_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

// 定义状态类型为C语言兼容
typedef enum {
    fault,
    startup,
    idle,
    listening,
    speaking
} ChatState;

// 创建并初始化Application对象
void* create_application(const char* address, int port, const char* token, const char* deviceId, int protocolVersion, int sample_rate, int channels, int frame_duration);

// 运行Application对象
void run_application(void* app_ptr);

// 获取当前状态
ChatState get_application_state(void* app_ptr);

// 销毁Application对象
void destroy_application(void* app_ptr);
#ifdef __cplusplus
}
#endif

#endif // APP_C_INTERFACE_H