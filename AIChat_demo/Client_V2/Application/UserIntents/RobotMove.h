// RobotControl.h
#ifndef ROBOT_MOVE_H
#define ROBOT_MOVE_H

#include <nlohmann/json.hpp>

namespace RobotMove {
    void Move(const nlohmann::json& arguments);
}

#endif // ROBOT_MOVE_H