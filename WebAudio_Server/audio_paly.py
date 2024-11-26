import numpy as np
import sounddevice as sd

# 读取并播放 PCM 文件
with open("./test_1.pcm", "rb") as f:
    audio_data = np.frombuffer(f.read(), dtype=np.int16)

# 播放，需指定采样率
sd.play(audio_data, samplerate=16000)
sd.wait()  # 等待播放完成
