#include "Handler.h"
#include "../Utils/user_log.h"

void Handler::ws_msg_handle(const std::string& message, bool is_binary, Application* app) {
    // 接收到json消息时的回调
    if(!is_binary) {
        Json::Value root;
        Json::Reader reader;
        // 解析 JSON 字符串
        bool parsingSuccessful = reader.parse(message, root);
        if (!parsingSuccessful) {
            USER_LOG_WARN("Error parsing message: %s", reader.getFormattedErrorMessages().c_str());
            app->eventQueue_.Enqueue(static_cast<int>(AppEvent::fault_happen));
        }
        // 获取 JSON 对象中的值
        const Json::Value type = root["type"];
        if (type.isString()) {
            std::string typeStr = type.asString();
            if (typeStr == "vad") {
                handle_vad_message(root, app);
            } else if (typeStr == "asr") {
                handle_asr_message(root, app);
            } else if (typeStr == "chat") {
                handle_chat_message(root, app);
            } else if (typeStr == "tts") {
                handle_tts_message(root, app);
            } else if (typeStr == "error") {
                USER_LOG_ERROR("server erro msg: %s", message.c_str());
                app->eventQueue_.Enqueue(static_cast<int>(AppEvent::fault_happen));
            }
        }
        USER_LOG_WARN("not event message type: %s", message.c_str());
    } else {    
        // 接收到二进制数据时的回调
        USER_LOG_INFO("Received binary message.");
        BinProtocolInfo protocol_info;
        std::vector<uint8_t> opus_data;
        std::vector<int16_t> pcm_data;

        // 解包二进制数据
        if(app->audio_processor_.UnpackBinFrame(reinterpret_cast<const uint8_t*>(message.data()), message.size(), protocol_info, opus_data)) {
            // 检查版本和类型是否符合预期
            if(protocol_info.version == app->get_ws_protocolVersion() && protocol_info.type == 0) {
                // 将解码后的Opus数据放入队列供播放器使用
                app->audio_processor_.decode(opus_data.data(), opus_data.size(), pcm_data);
                app->audio_processor_.addFrameToPlaybackQueue(pcm_data);
            } else {
                USER_LOG_WARN("Received frame with unexpected version or type");
            }
        } else {
            USER_LOG_WARN("Failed to unpack binary frame");
        }
    }

}

// 处理 VAD 消息
void Handler::handle_vad_message(const Json::Value& root, Application* app) {
    const Json::Value state = root["state"];
    if (state.isString()) {
        std::string stateStr = state.asString();
        if (stateStr == "no_speech") {
            app->eventQueue_.Enqueue(static_cast<int>(AppEvent::vad_no_speech));
        }
    }
}

// 处理 ASR 消息
void Handler::handle_asr_message(const Json::Value& root, Application* app) {
    const Json::Value text = root["text"];
    if (text.isString()) {
        std::string asr_text_ = text.asString();
        USER_LOG_INFO("Received ASR text: %s", asr_text_.c_str());
    } else {
        USER_LOG_WARN("Invalid ASR text value.");
    }
    app->eventQueue_.Enqueue(static_cast<int>(AppEvent::asr_received));
}

// 处理 TTS 消息
void Handler::handle_tts_message(const Json::Value& root, Application* app) {
    const Json::Value state = root["state"];
    if (state.isString()) {
        std::string stateStr = state.asString();
        if (stateStr == "end") {
            USER_LOG_INFO("Received TTS end.");
            app->set_tts_completed(true);
        }
    }
}

// 处理Chat消息
void Handler::handle_chat_message(const Json::Value& root, Application* app) {
    const Json::Value dialogue = root["dialogue"];
    if (dialogue.isString()) {
        std::string dialogueStr = dialogue.asString();
        if (dialogueStr == "end") {
            USER_LOG_INFO("Received dialogue end.");
            app->set_dialogue_completed(true);
        }
    }
}