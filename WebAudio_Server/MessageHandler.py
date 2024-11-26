import json
import logging
import os
from AudioProcessor import AudioProcessor

# 从环境变量中读取配置参数
SAMPLE_RATE = int(os.getenv('SAMPLE_RATE', 16000))
CHANNELS = int(os.getenv('CHANNELS', 1))
FRAME_DURATION_MS = int(os.getenv('FRAME_DURATION_MS', 20))

# 初始化音频处理器
audio_processor = AudioProcessor(SAMPLE_RATE, CHANNELS, FRAME_DURATION_MS)

# 配置日志
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)


# 处理文本数据
async def handle_text_message(data):
    if data.get('type') == 'hello':
        audio_params = data.get('audio_params', {})
        sample_rate = audio_params.get('sample_rate', 16000)
        channels = audio_params.get('channels', 1)
        frame_duration_ms = audio_params.get('frame_duration', 20)
        logger.info(f"Set audio parameters: sample_rate={sample_rate}, channels={channels}, frame_duration_ms={frame_duration_ms}")

    # 处理消息并发送响应
    response = {
        "type": "reply",
        "message": "OK"
    }
    return response

# 处理所有消息
async def handle_message(message):
    if isinstance(message, bytes):  # 如果消息是二进制数据
        bin_protocol =audio_processor.unpack_bin_frame(message)
        if bin_protocol:
            version, type, payload = bin_protocol
            if str(version) == "1" and type == 0:
                # 处理音频数据
                response = await handle_audio_data(payload)
                if response is not None:
                    return response
            else:
                logger.error("Unsupported protocol version or message type")
        else:
            logger.error("Failed to unpack binary frame")

    else:  # 如果消息是文本数据
        data = json.loads(message)
        logger.info(f"Received valid JSON message: {data}")
        response = await handle_text_message(data)
        if response is not None:
            return response

    return None
