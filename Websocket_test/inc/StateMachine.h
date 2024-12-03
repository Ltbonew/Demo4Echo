#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <string>
#include <functional>
#include <unordered_map>

class StateMachine {
public:
    StateMachine();
    void AddState(const std::string& state_name, const std::function<void()>& state_function);
    void SetInitialState(const std::string& state_name);
    void TransitionTo(const std::string& state_name);
    void Log(const std::string& message); // 新增的 Log 方法
    std::string GetCurrentState() const;

private:
    std::string current_state_;
    std::unordered_map<std::string, std::function<void()>> states_;
};

#endif // STATE_MACHINE_H