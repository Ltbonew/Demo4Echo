### 以下是server端发送的信息

鉴权

```json
{
    "type": "auth",
    "message": "Authentication failed" //还有"Client authenticated"
}
```

VAD检测到说话的活跃状态

```json
{
    "type": "vad",
    "state": "no_speech" // 还有end, too_long
}
```

ASR识别到说话的文字

```json
{
    "type": "asr",
    "text": "speech的内容"
}
```

tts生成语音完毕

```json
{
    "type": "tts",
    "text": "end"
}
```
