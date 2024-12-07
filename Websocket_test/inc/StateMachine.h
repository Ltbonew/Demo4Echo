#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include <functional>
#include <unordered_map>
#include <string>

using EnterFunc_t = std::function<void()>;
using ExitFunc_t = std::function<void()>;

// StateMachine class declaration
class StateMachine {
public:
    StateMachine(int initialState);
    ~StateMachine();

    void RegisterState(int state, EnterFunc_t on_enter, ExitFunc_t on_exit);
    void RegisterTransition(int from, int event, int to);
    bool HandleEvent(int event);

    int GetCurrentState() const;

    enum LogLevel { INFO, ERROR, WARNING };
    void Log(const std::string& message, LogLevel level = INFO);

private:
    void ChangeState(int newState);

    int currentState_;
    std::unordered_map<int, std::pair<EnterFunc_t, ExitFunc_t>> stateActions_;
    std::unordered_map<int, std::unordered_map<int, int>> transitions_;
};

#endif  // STATE_MACHINE_H_