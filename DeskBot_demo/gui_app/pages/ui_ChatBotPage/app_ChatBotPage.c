#include <stdio.h>
#include <pthread.h>
#include "ui_ChatBotPage.h"
#include "app_ChatBotPage.h"
#include "../../../../AIChat_demo/Client/c_interface/AIchat_c_interface.h"

static void* app_instance = NULL;
static pthread_t ai_chat_thread;
static int ai_chat_running = 0;
pthread_mutex_t running_mutex = PTHREAD_MUTEX_INITIALIZER;  // 保护状态访问的互斥锁


void* ai_chat_thread_func(void* arg) {
    if (app_instance == NULL) {
        LV_LOG_ERROR("AI Chat App 未初始化！\n");
        pthread_mutex_lock(&running_mutex);
        ai_chat_running = 0;
        pthread_mutex_unlock(&running_mutex);
        return NULL;
    }

    LV_LOG_USER("AI Chat App start running...");
    run_aichat_app(app_instance);  // 运行 AI Chat
    LV_LOG_USER("AI Chat App run end");

    pthread_mutex_lock(&running_mutex);
    ai_chat_running = 0;
    pthread_mutex_unlock(&running_mutex);

    if (app_instance) {
        destroy_aichat_app(app_instance);
        app_instance = NULL;
        LV_LOG_USER("AI Chat App already destroyed.");
    }

    pthread_mutex_lock(&running_mutex);
    ai_chat_thread = 0;  // **标记线程无效**
    pthread_mutex_unlock(&running_mutex);

    return NULL;
}


int start_ai_chat(const char* address, int port, const char* token, const char* deviceId, 
                  const char* aliyun_api_key, int protocolVersion, int sample_rate, 
                  int channels, int frame_duration) {

    pthread_mutex_lock(&running_mutex);
    // **如果上次线程还在运行，先等待它退出**
    if (ai_chat_thread) {
        LV_LOG_ERROR("last AI Chat thread not exit...");
        pthread_mutex_unlock(&running_mutex);
        return -1;
    }

    if (ai_chat_running) {
        LV_LOG_WARN("AI Chat already running.");
        pthread_mutex_unlock(&running_mutex);
        return -2;
    }
    pthread_mutex_unlock(&running_mutex);

    // 确保 app 已完全释放
    if (app_instance != NULL) {
        LV_LOG_ERROR("last AI Chat app not destroyed.");
        destroy_aichat_app(app_instance);
        app_instance = NULL;
        return -3;
    }

    // 创建应用
    app_instance = create_aichat_app(address, port, token, deviceId, aliyun_api_key, 
                                     protocolVersion, sample_rate, channels, frame_duration);
    if (app_instance == NULL) {
        LV_LOG_ERROR("Failed to create application.");
        return -4;
    }

    pthread_mutex_lock(&running_mutex);
    ai_chat_running = 1;
    pthread_mutex_unlock(&running_mutex);

    if (pthread_create(&ai_chat_thread, NULL, ai_chat_thread_func, NULL) != 0) {
        LV_LOG_ERROR("creat AI Chat thread fail");
        pthread_mutex_lock(&running_mutex);
        ai_chat_running = 0;
        pthread_mutex_unlock(&running_mutex);
        destroy_aichat_app(app_instance);
        app_instance = NULL;
        return -5;
    }

    return 0;
}

void stop_ai_chat(void) {
    if (app_instance == NULL) {
        LV_LOG_WARN("AI Chat not init\n");
        return;
    }

    pthread_mutex_lock(&running_mutex);
    if (ai_chat_running) {
        stop_aichat_app(app_instance);
    }
    pthread_mutex_unlock(&running_mutex);
}

// 获取 AI Chat 状态
int get_ai_chat_state(void) {
    if (app_instance == NULL) {
        return -1;
    }
    return get_aichat_app_state(app_instance);  // 例如返回 0 (idle), 1 (listening), 2 (speaking)
}