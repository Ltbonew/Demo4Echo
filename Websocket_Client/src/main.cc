#include "../inc/Application.h"

int main() {
    // 定义server地址、端口和鉴权头
    std::string address = "192.168.211.1";
    int port = 8765;
    std::string token = "123456";
    std::string deviceId = "00:11:22:33:44:55";
    std::string protocolVersion = "1";
    int sample_rate = 16000;
    int channels = 1;
    int frame_duration = 40;

    // 创建 Application 实例
    Application app(address, port, token, deviceId, protocolVersion, sample_rate, channels, frame_duration);

    // 运行应用程序
    app.Run();

    return 0;
}