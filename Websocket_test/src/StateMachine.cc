#include "../inc/StateMachine.h"
#include <iostream>

StateMachine::StateMachine(int initialState)
    : currentState_(initialState) {
        Log("State machine created.");
    }

StateMachine::~StateMachine() {
    Log("State machine destroyed.");
}

void StateMachine::Initialize() {
        // 调用初始状态的 enter 回调
        if (stateActions_.find(currentState_) != stateActions_.end()) {
            stateActions_[currentState_].first();
        }
    }

void StateMachine::RegisterState(int state, EnterFunc_t on_enter, ExitFunc_t on_exit) {
    stateActions_[state] = std::make_pair(on_enter, on_exit);
}

void StateMachine::RegisterTransition(int from, int event, int to) {
    transitions_[from][event] = to;
}

bool StateMachine::HandleEvent(int event) {
    auto& possibleTransitions = transitions_[currentState_];
    if (possibleTransitions.find(event) == possibleTransitions.end()) {
        Log("Event not handled in current state.", LogLevel::ERROR);
        return false;
    }

    int nextState = possibleTransitions[event];
    ChangeState(nextState);
    return true;
}

int StateMachine::GetCurrentState() const {
    return currentState_;
}

void StateMachine::ChangeState(int newState) {
    // Call exit function for the current state if it exists
    if (stateActions_.find(currentState_) != stateActions_.end()) {
        stateActions_[currentState_].second();
    }

    // Update the current state
    currentState_ = newState;

    // Call enter function for the new state if it exists
    if (stateActions_.find(currentState_) != stateActions_.end()) {
        stateActions_[currentState_].first();
    }
}

void StateMachine::Log(const std::string& message, LogLevel level) {
    // 根据日志级别选择前缀
    std::string prefix;
    switch (level) {
        case LogLevel::INFO:
            prefix = "[INFO] ";
            break;
        case LogLevel::WARNING:
            prefix = "[WARNING] ";
            break;
        case LogLevel::ERROR:
            prefix = "[ERROR] ";
            break;
        default:
            prefix = "[UNKNOWN] ";
            break;
    }

    // 输出日志信息
    std::cout << "[StateMachine] " << prefix << message << std::endl;
}