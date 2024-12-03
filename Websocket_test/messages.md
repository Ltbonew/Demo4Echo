### 以下是client端发送的信息

连接server需要鉴权的信息

```cpp
std::string token = "123456";
std::string deviceId = "00:11:22:33:44:55";
std::string protocolVersion = "1";
```

发送hello & 参数

```json
{
    "type": "hello",
    "audio_params": {
        "format": "opus",
        "sample_rate": "16000",
        "channels": "1",
        "frame_duration": "40"
    }
}
```

状态改变

```json
{
    "type": "state", 
    "state": "idle"
}
```

音频数据

```cpp
struct BinProtocol {
    uint16_t version;       //协议版本
    uint16_t type;          //0为音频数据
    uint32_t payload_size;  //音频数据长度
    uint8_t payload[];      //opus音频数据
} __attribute__((packed));
```