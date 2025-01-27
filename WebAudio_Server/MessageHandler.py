import json
import logging
from collections import deque
from AudioProcessor import AudioProcessor
from ModelManager import ModelManager
import WebsocketServer
import numpy as np

# 配置日志
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

class MessageHandler:
    def __init__(self, protocol_version="1"):
        self.protocol_version = protocol_version
        # 缓冲区设置
        self.BUFFER_MAX_LENGTH_MS = 10 * 1000  # 缓冲区最大长度
        self.VAD_FRAME_DURATION_MS = 200  # VAD 推理的片段长度
        self.NO_SPEECH_TIMEOUT_MS = 3000  # 无语音活动超时
        self.POST_SPEECH_BUFFER_MS = 200  # 语音结束后的判断结束的时间
        # 初始化音频处理器
        self.audio_processor = AudioProcessor()
        self.SAMPLE_RATE, self.CHANNALS, self.FRAME_DURATION_MS = self.audio_processor.get_audio_params()
        # 创建模型
        self.model_manager = ModelManager()
        # 使用numpy数组作为音频缓冲区
        self.audio_buffer = np.array([], dtype=np.int16)  # 初始化为空的numpy数组
        # 记录VAD处理的最后位置，用于200ms的VAD推理
        self.vad_process_pos = 0  # 毫秒
        # 记录开始语音活动的位置
        self.start_speech_pos = -1  # 毫秒
        # 记录最后一次语音活动的位置
        self.last_speech_pos = -1  # 毫秒
        # 记录是否在说话
        self.is_speaking = False

    def audio_proc_reset(self):
        self.vad_process_pos = 0    # 记录VAD处理的最后位置, 用于200ms的VAD推理
        self.audio_buffer = np.array([], dtype=np.int16)  # 初始化为空的numpy数组
        self.is_speaking = False
        self.start_speech_pos = -1  # 毫秒
        self.last_speech_pos = -1  # 毫秒
        self.model_manager.VAD_cache_clean()
        logger.info("Audio processor reset")

    def VAD_proc_audio_stream(self):
        # 计算音频数据的时长
        audio_length = len(self.audio_buffer) * 1000 // self.SAMPLE_RATE
        # 使用 VAD 推理，处理200ms片段对应数据长度
        chunk_stride = self.SAMPLE_RATE * self.VAD_FRAME_DURATION_MS // 1000
        # 不足200ms的音频数据，不进行VAD处理
        if audio_length - self.vad_process_pos < self.VAD_FRAME_DURATION_MS:
            return None
        # 获取下一个的200ms音频片段
        # speech_chunk = self.audio_buffer[-chunk_size*self.SAMPLE_RATE//1000:]
        beg_frame = self.vad_process_pos*self.SAMPLE_RATE//1000
        end_frame = (self.vad_process_pos)*self.SAMPLE_RATE//1000 + chunk_stride
        speech_chunk = self.audio_buffer[beg_frame : end_frame]
        # VAD推理
        start, end = self.model_manager.VAD_Detection(speech_chunk)
        # 更新VAD处理的位置
        self.vad_process_pos += self.VAD_FRAME_DURATION_MS  # 处理了一个200ms片段
        logger.info("start: %s, end: %s", start, end)
        # 处理VAD结果, 判定当前状态
        if end is not None:
            self.start_speech_pos = 0
            # 检测到语音结束点为一个数字ms
            if(end != -1):
                self.last_speech_pos = end
                self.is_speaking = False
            # -1表示没检测到结束点, 仍在说话
            else:
                self.is_speaking = True
        # 如果一直在说话, last_speech_pos那就是当前的音频长度
        if self.is_speaking:
            self.last_speech_pos = audio_length
        # 如果超过等待说话时间3s都一直都没有说话
        if self.start_speech_pos == -1:
            if audio_length > self.NO_SPEECH_TIMEOUT_MS:
                logger.info("No speech detected for %d seconds, drop...", self.NO_SPEECH_TIMEOUT_MS // 1000)
                res = {
                    "type": "vad",
                    "state": "no_speech"
                }
                return res

        # 判断是否达到10s的缓冲区最大长度，若达到，则触发ASR识别
        if audio_length > self.BUFFER_MAX_LENGTH_MS:
            logger.info("Buffer exceeded %d seconds", self.BUFFER_MAX_LENGTH_MS // 1000)
            res = {
                "type": "vad",
                "state": "too_long"
            }
            return res

        # 判断是否超过1s未检测到语音活动，若超过则触发ASR识别
        if self.last_speech_pos > 0 and (audio_length - self.last_speech_pos) > self.POST_SPEECH_BUFFER_MS:
            logger.info("end speech position: %d", self.last_speech_pos)
            logger.info("Post-speech buffer exceeded 1 second")
            res = {
                "type": "vad",
                "state": "end"
            }
            return res
        return None

    # 处理音频数据
    async def handle_audio_data(self, payload):
        # 解码音频数据
        pcm_data = self.audio_processor.decode_audio(payload)
        audio_data_array = np.frombuffer(pcm_data, dtype=np.int16)
        # 将解码后的音频数据添加到缓冲区
        self.audio_buffer = np.append(self.audio_buffer, audio_data_array)
        # 处理音频流
        res = self.VAD_proc_audio_stream()
        return res

    # 处理文本数据
    async def handle_text_message(self, data):
        if data.get('type') == 'hello':
            self.audio_proc_reset()
            audio_params = data.get('audio_params', {})
            # 暂时没设定可变的音频参数列表, 所以client发送过来的音频参数不会被使用
            # sample_rate = audio_params.get('sample_rate', AudioProcessor.SAMPLE_RATE)
            # channels = audio_params.get('channels', AudioProcessor.CHANNELS)
            # frame_duration_ms = audio_params.get('frame_duration', AudioProcessor.FRAME_DURATION_MS)
            # logger.info(f"Set audio parameters: sample_rate={sample_rate}, channels={channels}, frame_duration_ms={frame_duration_ms}")
            # self.audio_processor.set_audio_params(sample_rate, channels, frame_duration_ms)

        if data.get('type') == 'state' :
            if data.get('state') == 'idle':
                self.audio_proc_reset()

        if data.get('type') == 'state' :
            if data.get('state') == 'listening':
                self.audio_proc_reset()

            elif data.get('state') == 'thinking':
                asr_text = self.model_manager.ASR_generate_text(self.audio_buffer[:self.last_speech_pos*self.SAMPLE_RATE//1000].astype(np.float32))
                if asr_text:
                    logger.info(f"ASR result: {asr_text}")
                    # asr识别异常处理还需要
                    response =  {
                        "type": "asr",
                        "text": asr_text
                    }
                    self.audio_proc_reset()
                    return response
        return None
    # 处理所有消息
    async def handle_message(self, message):
        if isinstance(message, bytes):  # 如果消息是二进制数据
            bin_protocol = self.audio_processor.unpack_bin_frame(message)
            if bin_protocol:
                version, type, payload = bin_protocol
                if str(version) == self.protocol_version and type == 0:
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
