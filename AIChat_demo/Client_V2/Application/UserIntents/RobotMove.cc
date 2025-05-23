// RobotControl.cc
#include "RobotMove.h"
#include <iostream>

namespace RobotMove {
    void Move(const nlohmann::json& arguments) {
        std::string direction = arguments["direction"];
        std::cout << "Robot moving " << direction << std::endl;
    }
}