#ifndef HANDLER_H
#define HANDLER_H

#include <string>
#include "Application.h"
#include <json/json.h>

class Handler {
public:
    // 处理 WebSocket 接收到的消息
    void ws_msg_handle(const std::string& message, bool is_binary, Application* app);
private:
    void handle_vad_message(const Json::Value& root, Application* app);
    void handle_asr_message(const Json::Value& root, Application* app);
    void handle_chat_message(const Json::Value& root, Application* app);
    void handle_tts_message(const Json::Value& root, Application* app);
};

#endif // HANDLER_H