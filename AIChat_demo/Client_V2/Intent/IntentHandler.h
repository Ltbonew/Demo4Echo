// IntentHandler.h
#ifndef INTENT_HANDLER_H
#define INTENT_HANDLER_H

#include <string>
#include <unordered_map>
#include <functional>
#include <nlohmann/json.hpp> // 使用 JSON 库处理数据

class IntentHandler {
public:
    using Callback = std::function<void(const nlohmann::json& arguments)>;

    // 注册回调函数
    void RegisterFunction(const std::string& function_name, Callback callback);

    // 处理服务器发送的意图数据
    void HandleIntent(const nlohmann::json& intent_data);

private:
    std::unordered_map<std::string, Callback> function_map_; // 存储 function_name 和回调函数的映射
};

#endif // INTENT_HANDLER_H