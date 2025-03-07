#include <stdio.h>
#include <pthread.h>
#include "app_ChatBotPage.h"
#include "../../../../AIChat_demo/Client/c_interface/AIchat_c_interface.h"

static void* app_instance = NULL;
static pthread_t ai_chat_thread;
static int ai_chat_running = 0;
static int app_state = 0;  // 用于存储应用状态
pthread_mutex_t running_mutex = PTHREAD_MUTEX_INITIALIZER;  // 保护状态访问的互斥锁


void* ai_chat_thread_func(void* arg) {
    if (app_instance == NULL) {
        printf("AI Chat App 未初始化！\n");
        ai_chat_running = 0;
        return NULL;
    }

    printf("AI Chat App 开始运行...\n");
    run_aichat_app(app_instance);  // 运行 AI Chat
    printf("AI Chat App 运行结束。\n");

    pthread_mutex_lock(&running_mutex);
    ai_chat_running = 0;
    pthread_mutex_unlock(&running_mutex);

    
    return NULL;
}

int start_ai_chat(const char* address, int port, const char* token, const char* deviceId, const char* aliyun_api_key, 
                    int protocolVersion, int sample_rate, int channels, int frame_duration) {
    if (ai_chat_running) {
        printf("AI Chat 已经在运行。\n");
        return -1;
    }

    // 创建应用
    app_instance = create_aichat_app(address, port, token, deviceId, aliyun_api_key, protocolVersion, sample_rate, channels, frame_duration);
    if (app_instance == NULL) {
        printf("Failed to create application.\n");
        return -1;
    }
    
    pthread_mutex_lock(&running_mutex);
    ai_chat_running = 1;
    pthread_mutex_unlock(&running_mutex);

    if (pthread_create(&ai_chat_thread, NULL, ai_chat_thread_func, NULL) != 0) {
        printf("创建 AI Chat 线程失败。\n");
        pthread_mutex_lock(&running_mutex);
        ai_chat_running = 0;
        pthread_mutex_unlock(&running_mutex);
        destroy_aichat_app(app_instance);
        app_instance = NULL;
    }
    return 0;
}

// 获取 AI Chat 状态
int get_ai_chat_state(void) {
    if (app_instance == NULL) {
        return 0;  // idle
    }
    return get_aichat_app_state(app_instance);  // 例如返回 0 (idle), 1 (listening), 2 (speaking)
}