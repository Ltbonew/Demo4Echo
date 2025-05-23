// RobotMove.cc
#include "RobotMove.h"
#include <iostream>

namespace RobotMove {
    void Move(const Json::Value& arguments) {
        // 从 arguments 中获取 "direction" 参数
        if (arguments.isMember("direction") && arguments["direction"].isString()) {
            std::string direction = arguments["direction"].asString();
            std::cout << "Robot moving " << direction << std::endl;
        } else {
            std::cerr << "Invalid or missing 'direction' argument in RobotMove::Move" << std::endl;
        }
    }
}