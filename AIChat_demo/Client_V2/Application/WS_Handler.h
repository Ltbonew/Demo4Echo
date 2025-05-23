#ifndef WS_HANDLER_H
#define WS_HANDLER_H

#include <string>
#include "Application.h"
#include <json/json.h>

class WSHandler {
public:
    // 处理 WebSocket 接收到的消息
    void ws_msg_handle(const std::string& message, bool is_binary, Application* app);
private:
    void handle_vad_message(const Json::Value& root, Application* app);
    void handle_asr_message(const Json::Value& root, Application* app);
    void handle_chat_message(const Json::Value& root, Application* app);
    void handle_tts_message(const Json::Value& root, Application* app);
};

#endif // WS_HANDLER_H