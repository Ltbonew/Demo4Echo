#include "WebsocketClient.h"
#include "StateMachine.h"
#include "AudioProcess.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>


int main() {
    // 定义地址、端口和鉴权头
    std::string address = "192.168.211.1";
    int port = 8765;
    std::string token = "123456";
    std::string deviceId = "00:11:22:33:44:55";
    std::string protocolVersion = "1";

    int sample_rate = 16000;
    int channels = 1;
    int frame_duration = 20;

    // 创建 WebSocketClient 实例
    WebSocketClient ws_client(address, port, token, deviceId, protocolVersion);

    // 设置接收到消息的回调函数
    ws_client.SetMessageCallback([&ws_client](const std::string& message) {
        ws_client.Log("Received message: " + message); // 使用 ws_client 的 Log 方法
    });

    // 创建一个线程来连接服务器
    std::thread ws_thread([&ws_client]() {
        ws_client.Connect();
    });
    
    while(!ws_client.IsConnected()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ws_client.Log("Waiting for connection...");
    }

    if(ws_client.IsConnected())
    {

        //
        std::string json_message = 
        R"({
            "type": "hello",
            "audio_params": {
                "format": "opus",
                "sample_rate": )" + std::to_string(sample_rate) + R"(,
                "channels": )" + std::to_string(channels) + R"(,
                "frame_duration": )" + std::to_string(frame_duration) + R"(
            }
        })";

        ws_client.SendText(json_message);

        AudioProcess audio_processor(sample_rate, channels);
        std::queue<std::vector<int16_t>> audio_queue_ = audio_processor.loadAudioFromFile("../test_audio/test.pcm", frame_duration);
        
        while(!audio_queue_.empty())
        {
            std::vector<int16_t> pcm_frame = audio_queue_.front();
            audio_queue_.pop();

            uint8_t opus_data[1536];
            size_t opus_data_size;

            if (audio_processor.encode(pcm_frame, opus_data, opus_data_size)) {
                // 打包
                BinProtocol* packed_frame = audio_processor.PackBinFrame(opus_data, opus_data_size);

                if (packed_frame) {
                    // 发送
                    ws_client.SendBinary(reinterpret_cast<uint8_t*>(packed_frame), sizeof(BinProtocol) + opus_data_size);
                } else {
                    audio_processor.Log("Packing failed", audio_processor.ERROR);
                }
            } else {
                audio_processor.Log("Encoding failed", audio_processor.ERROR);
            }
        }
        std::cerr << "finish" << std::endl;

    }
    // 等待 WebSocket 线程结束
    ws_thread.join();

    return 0;
}
