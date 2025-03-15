#ifndef __APP_CHATBOTPAGE_H
#define __APP_CHATBOTPAGE_H

#ifdef __cplusplus
extern "C" {
#endif

// 创建并初始化Application对象, 会开启一个线程
int start_ai_chat(const char* address, int port, const char* token, const char* deviceId, const char* aliyun_api_key, 
                    int protocolVersion, int sample_rate, int channels, int frame_duration);
// 获取 AI Chat 状态
int get_ai_chat_state(void);

void move_cmd_callback(const char * str);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif