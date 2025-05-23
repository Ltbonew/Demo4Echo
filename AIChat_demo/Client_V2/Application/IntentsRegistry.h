#ifndef INTENTS_REGISTRY_H
#define INTENTS_REGISTRY_H

#include "../Intent/IntentHandler.h"
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <string>

class IntentsRegistry {
public:
    // 汇总注册所有函数到 IntentHandler
    static void RegisterAllFunctions(IntentHandler& intent_handler);

    // 生成注册消息的 JSON
    static nlohmann::json GenerateRegisterMessage();
};

#endif // INTENTS_REGISTRY_H