from funasr import AutoModel
from funasr.utils.postprocess_utils import rich_transcription_postprocess
import logging
import dashscope
from dashscope.api_entities.dashscope_response import SpeechSynthesisResponse
from dashscope.audio.tts_v2 import *
import time

# 配置日志
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

# 初始化模型管理器路径
vad_model = "./FunAudioLLM/iic/speech_fsmn_vad_zh-cn-16k-common-pytorch"
asr_model = "./FunAudioLLM/iic/SenseVoiceSmall"
remote_code = "./FunAudioLLM/SenseVoice/model.py"
# 设置 阿里百炼 API Key
dashscope.api_key = "your-api-key"

class ModelManager:
    def __init__(self, vad_model_dir=vad_model, asr_model_dir=asr_model, remote_code_dir=remote_code, device="cuda:0"):
        # 初始化 VAD 模型
        self.vad_model = AutoModel(
            model=vad_model_dir,
            disable_pbar=True,
            max_end_silence_time=600,
            # vad_kwargs={"max_single_segment_time": 500},
            disable_update=True
        )
        self.vad_cache = {}

        # 初始化 ASR 模型
        self.asr_model = AutoModel(
            model=asr_model_dir,
            remote_code=remote_code_dir,
            trust_remote_code=True,
            device=device,
            disable_update=True
        )

        # 对话初始历史记录
        self.messages = [
            {'role': 'system', 'content': 'You are a desk assistant, answering questions shortly and quickly.'}
        ]

    # VAD init
    def VAD_cache_clean(self):
        self.vad_cache = {}

    # VAD
    def VAD_Detection(self, speech_chunk, chunk_size=200):
        """
        [[beg, -1]]：表示只检测到起始点。
        [[-1, end]]：表示只检测到结束点。
        []：表示既没有检测到起始点，也没有检测到结束点 输出结果单位为毫秒，从起始点开始的绝对时间
        """
        res = self.vad_model.generate(input=speech_chunk, cache=self.vad_cache, is_final=False, chunk_size=chunk_size)
        if len(res[0]["value"]):
            start = res[0]["value"][0][0]
            end = res[0]["value"][0][1]
            return start, end
        return None, None

    def ASR_generate_text(self, audio_buffer):
        res = self.asr_model.generate(input=audio_buffer, cache={}, language='auto', use_itn=True)
        # res是有情感等信息的, 只取text
        if(res[0]['text']):
            return rich_transcription_postprocess(res[0]['text'])
        return None

    def add_message(self, role, content):
        """添加对话记录"""
        self.messages.append({'role': role, 'content': content})

    def clear_messages(self):
        """清除对话记录"""
        self.messages.clear()

    def get_LLM_answer(self, question):
        """获取百炼对话回答\r\n
        注意: 此处是yield生成器, 需要在外部循环中调用"""
        # 添加用户问题
        self.add_message('user', question)
        messages = self.messages

        responses = dashscope.Generation.call(
            # 若没有配置环境变量，请用百炼API Key将下行替换为：api_key="sk-xxx",
            api_key=dashscope.api_key,
            model="qwen-turbo", # 模型列表：https://help.aliyun.com/zh/model-studio/getting-started/models
            messages=messages,
            result_format='message',
            stream=True,
            incremental_output=True
        )
        full_text = ''
        for response in responses:
            for choice in response["output"]["choices"]:
                full_text += choice["message"]["content"]
                yield choice["message"]["content"]
        # 最后记录回复的信息
        self.add_message('assistant', full_text)

    class __tts_callback(ResultCallback):

        def on_open(self):
            logger.info("tts server-WS is open.")

        def on_complete(self):
            logger.info("Speech synthesis task completed successfully.")

        def on_error(self, message: str):
            logger.info(f"Speech synthesis task failed, {message}")

        def on_close(self):
            logger.info("tts server-WS is closed.")

        def on_data(self, data: bytes) -> None:
            logger.info("Audio result length:", len(data))
            self._stream.write(data)

    def tts_stream_speech_synthesis(self, text_generator):
        '''流式语音合成\r\n
        可以直接将get_LLM_answer()的输出作为该函数的输入, 已经有循环调用yield\r\n
        text_generator: 生成器或一段文本列表'''
        callback = self.__tts_callback()
        synthesizer = SpeechSynthesizer(
            model="cosyvoice-v1",
            voice="longxiaochun",
            format=AudioFormat.PCM_16000HZ_MONO_16BIT,
            callback=callback,
        )

        for text_chunk in text_generator:
            if text_chunk:
                synthesizer.streaming_call(text_chunk)
                time.sleep(0.5)  # 简单模拟延迟

        synthesizer.streaming_complete()
        print('Request ID:', synthesizer.get_last_request_id())
