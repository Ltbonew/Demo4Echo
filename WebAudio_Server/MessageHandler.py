import json
import logging
import os
from collections import deque
from AudioProcessor import AudioProcessor
from ModelManager import ModelManager
import WebsocketServer

# Audio配置参数
SAMPLE_RATE = 16000
CHANNELS = 1
FRAME_DURATION_MS = 20

# 初始化模型管理器路径
vad_model_dir = "./FunAudioLLM/iic/speech_fsmn_vad_zh-cn-16k-common-pytorch"
asr_model_dir = "./FunAudioLLM/iic/SenseVoiceSmall"
remote_code_dir = "./FunAudioLLM/SenseVoice/model.py"

# 配置日志
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

class MessageHandler:
    def __init__(self):
        # 缓冲区设置
        self.BUFFER_MAX_LENGTH_S = 15  # 缓冲区最大长度（秒）
        self.BUFFER_MAX_LENGTH_SAMPLES = self.BUFFER_MAX_LENGTH_S * SAMPLE_RATE // 1000 * FRAME_DURATION_MS
        self.VAD_FRAME_DURATION_MS = 240  # VAD 推理的片段长度（毫秒）
        # 初始化音频处理器
        self.audio_processor = AudioProcessor(SAMPLE_RATE, CHANNELS, FRAME_DURATION_MS)
        # 创建模型
        self.model_manager = ModelManager(vad_model_dir, asr_model_dir, remote_code_dir)
        # 创建音频缓冲区
        self.audio_buffer = deque()

    # 处理文本数据
    async def handle_text_message(self, data):
        if data.get('type') == 'hello':
            audio_params = data.get('audio_params', {})
            sample_rate = audio_params.get('sample_rate', 16000)
            channels = audio_params.get('channels', 1)
            frame_duration_ms = audio_params.get('frame_duration', 20)
            logger.info(f"Set audio parameters: sample_rate={sample_rate}, channels={channels}, frame_duration_ms={frame_duration_ms}")
            self.audio_processor.set_audio_params(sample_rate, channels, frame_duration_ms)

        # 处理消息并发送响应
        response = {
            "type": "reply",
            "message": "OK"
        }
        return response

    # 处理音频数据
    async def handle_audio_data(self, payload):
        # 解码音频数据
        pcm_data =self. audio_processor.decode_audio(payload)

        # 将解码后的音频数据添加到缓冲区
        self.audio_buffer.extend(pcm_data)

        # 检查缓冲区是否超过最大长度
        while len(self.audio_buffer) > self.BUFFER_MAX_LENGTH_SAMPLES:
            self.audio_buffer.popleft()  # 移除最旧的数据

        # 运行 VAD 模型
        is_speaking = self.model_manager.run_vad(b''.join(self.audio_buffer), SAMPLE_RATE)

        # 如果 VAD 检测到说话中断或缓冲区已满，则进行 ASR 识别
        if not is_speaking or len(self.audio_buffer) >= self.BUFFER_MAX_LENGTH_SAMPLES:
            # 将缓冲区内容转换为 PCM 数据
            buffer_pcm_data = b''.join(self.audio_buffer)

            # 使用 ASR 模型识别音频段
            transcription = self.model_manager.run_asr(buffer_pcm_data, SAMPLE_RATE)

            # 清空缓冲区
            self.audio_buffer.clear()

            # 返回处理结果
            response = {
                "type": "transcription",
                "message": transcription
            }
            return response

        return None

    # 处理所有消息
    async def handle_message(self, message):
        if isinstance(message, bytes):  # 如果消息是二进制数据
            bin_protocol = self.audio_processor.unpack_bin_frame(message)
            if bin_protocol:
                version, type, payload = bin_protocol
                if str(version) == WebsocketServer.PROTOCOL_VERSION and type == 0:
                    # 处理音频数据
                    response = await self.handle_audio_data(payload)
                    if response is not None:
                        return response
                else:
                    logger.error("Unsupported protocol version or message type")
            else:
                logger.error("Failed to unpack binary frame")

        else:  # 如果消息是文本数据
            data = json.loads(message)
            logger.info(f"Received valid JSON message: {data}")
            response = await self.handle_text_message(data)
            if response is not None:
                return response

        return None
