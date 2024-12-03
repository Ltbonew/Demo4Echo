#include "../inc/StateMachine.h"
#include <iostream>

StateMachine::StateMachine()
    : current_state_("") {}

void StateMachine::AddState(const std::string& state_name, const std::function<void()>& state_function) {
    states_[state_name] = state_function;
}

void StateMachine::SetInitialState(const std::string& state_name) {
    if (states_.find(state_name) != states_.end()) {
        current_state_ = state_name;
        states_[current_state_]();
    } else {
        Log("Initial state '" + state_name + "' not found.");
    }
}

void StateMachine::TransitionTo(const std::string& state_name) {
    if (states_.find(state_name) != states_.end()) {
        current_state_ = state_name;
        states_[current_state_]();
    } else {
        Log("State '" + state_name + "' not found.");
    }
}

std::string StateMachine::GetCurrentState() const {
    return current_state_;
}

void StateMachine::Log(const std::string& message) {
    std::cout << "[StateMachine] " << message << std::endl; // 你可以选择使用 std::cerr 或其他日志系统
}