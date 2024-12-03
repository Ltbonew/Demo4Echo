#ifndef APPLICATION_H
#define APPLICATION_H

#include "WebsocketClient.h"
#include "StateMachine.h"
#include "AudioProcess.h"
#include <json/json.h>
#include <string>
#include <queue>
#include <vector>
#include <cstdint>
#include <chrono>
#include <thread>
#include <iostream>

class Application {
public:
    Application(const std::string& address, int port, const std::string& token, const std::string& deviceId, const std::string& protocolVersion, int sample_rate, int channels, int frame_duration);
    void Run();

private:
    void HandleVADMessage(const Json::Value& root);
    void HandleASRMessage(const Json::Value& root);
    void HandleMessage(const std::string& message);
    void IdleState();
    void ListeningState();
    void ThinkState();
    void SpeakingState();

    WebSocketClient ws_client_;
    StateMachine client_state_; // 更改成员变量名称
    int sample_rate_;
    int channels_;
    int frame_duration_;
};

#endif // APPLICATION_H