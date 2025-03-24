#include <stdio.h>
#include <pthread.h>
#include "ui_ChatBotPage.h"
#include "app_ChatBotPage.h"
#include "../../../../AIChat_demo/Client/c_interface/AIchat_c_interface.h"

static void* app_instance = NULL; // AI Chat 应用实例
static pthread_t ai_chat_thread;  // 线程 ID
static volatile int is_running = 0; // 标志位，表示应用是否正在运行


void* ai_chat_thread_func(void* arg) {
    if (app_instance) {
        run_aichat_app(app_instance); // 运行 AI Chat 应用
    }
    is_running = 0; // 线程结束时将标志位重置为 0
    destroy_aichat_app(app_instance);
    app_instance = NULL;
    is_running = 0;
    return NULL;
}

int start_ai_chat(const char* address, int port, const char* token, const char* deviceId, 
                  const char* aliyun_api_key, int protocolVersion, int sample_rate, 
                  int channels, int frame_duration) {

    // 如果应用已经在运行，返回错误
    if (is_running) {
        LV_LOG_ERROR("Error: AI Chat application is already running.\n");
        return -1;
    }

    // 创建 Application 实例
    app_instance = create_aichat_app(address, port, token, deviceId, aliyun_api_key,
                                     protocolVersion, sample_rate, channels, frame_duration);
    if (!app_instance) {
        LV_LOG_ERROR("Error: Failed to create AI Chat application instance.\n");
        return -1;
    }

    // 注册运动指令回调函数
    set_cmd_callback(app_instance, move_cmd_callback);

    // 启动线程运行 AI Chat 应用
    is_running = 1; // 设置运行标志位
    if (pthread_create(&ai_chat_thread, NULL, ai_chat_thread_func, NULL) != 0) {
        LV_LOG_ERROR("Error: Failed to create AI Chat thread.\n");
        destroy_aichat_app(app_instance); // 清理实例
        app_instance = NULL;
        is_running = 0;
        return -1;
    }

    return 0; // 成功启动
}

int stop_ai_chat(void) {
    // 如果应用没有运行，返回错误
    if (!is_running) {
        LV_LOG_ERROR("Error: AI Chat application is not running.\n");
        return -1;
    }
    // 发送停止信号给应用
    stop_aichat_app(app_instance);
}

// 获取 AI Chat 状态
int get_ai_chat_state(void) {
    // 如果应用没有运行，返回错误状态
    if (!is_running || !app_instance) {
        return -1; // 返回 -1 表示无效状态
    }
    // 获取当前状态
    return get_aichat_app_state(app_instance);
}

// 0 none, 1 forward, 2 back, 3 left, 4 right.
uint8_t chat_bot_move_dir = 0; 

// cmd callback
void move_cmd_callback(const char * str)
{
    if (strcmp(str, "__label__MoveForward") == 0) {
        // function move forward
        LV_LOG_INFO("Move forward.");
        chat_bot_move_dir = 1;
    }
    else if (strcmp(str, "__label__MoveBackward") == 0) {
        // function move backward
        LV_LOG_INFO("Move backward.");
        chat_bot_move_dir = 2;
    }
    else if (strcmp(str, "__label__MoveTurnLeft") == 0) {
        // function move turn left
        LV_LOG_INFO("Move turn left.");
        chat_bot_move_dir = 3;
    }
    else if (strcmp(str, "__label__MoveTurnright") == 0) {
        // function move turn right
        LV_LOG_INFO("Move turn right.");
        chat_bot_move_dir = 4;
    }
}
