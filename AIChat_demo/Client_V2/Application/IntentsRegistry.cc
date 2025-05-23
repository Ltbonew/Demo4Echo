#include "IntentsRegistry.h"
#include "UserIntents/RobotMove.h"

void IntentsRegistry::RegisterAllFunctions(IntentHandler& intent_handler) {
    // 注册机器人移动相关的函数
    intent_handler.RegisterFunction("robot_move", RobotMove::Move);

    // 如果有其他功能模块，可以在这里继续注册
    // intent_handler.RegisterFunction("audio_play", AudioControl::Play);
}

nlohmann::json IntentsRegistry::GenerateRegisterMessage() {
    nlohmann::json message;
    message["type"] = "functions_register";

    // 添加 robot_move 的元信息
    message["functions"].push_back({
        {"name", "robot_move"},
        {"description", "让机器人运动"},
        {"arguments", {
            {"direction", "字符数据,分别有forward,backward,left和right"}
        }}
    });

    // 如果有其他功能模块，可以在这里继续添加元信息
    // message["functions"].push_back({
    //     {"name", "audio_play"},
    //     {"description", "Play an audio file"},
    //     {"arguments", {
    //         {"file", "string"},
    //         {"volume", "int, optional"},
    //     }}
    // });

    return message;
}