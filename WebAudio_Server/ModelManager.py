from funasr import AutoModel
from funasr.utils.postprocess_utils import rich_transcription_postprocess

# 初始化模型管理器路径
vad_model = "./FunAudioLLM/iic/speech_fsmn_vad_zh-cn-16k-common-pytorch"
asr_model = "./FunAudioLLM/iic/SenseVoiceSmall"
remote_code = "./FunAudioLLM/SenseVoice/model.py"

class ModelManager:
    def __init__(self, vad_model_dir=vad_model, asr_model_dir=asr_model, remote_code_dir=remote_code, device="cuda:0"):
        # 初始化 VAD 模型
        self.vad_model = AutoModel(
            model=vad_model_dir,
            disable_pbar=True,
            max_end_silence_time=1000,
            vad_kwargs={"max_single_segment_time": 500},
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
        return rich_transcription_postprocess(res[0]['text'])
