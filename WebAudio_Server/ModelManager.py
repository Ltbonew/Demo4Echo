from funasr import AutoModel
from funasr.utils.postprocess_utils import rich_transcription_postprocess

class ModelManager:
    def __init__(self, vad_model_dir, asr_model_dir, remote_code_dir, device="cuda:0"):
        # 初始化 VAD 模型
        self.vad_model = AutoModel(
            model=vad_model_dir,
            disable_pbar=True,
            max_end_silence_time=1000,
            vad_kwargs={"max_single_segment_time": 500},
            disable_update=True
        )

        # 初始化 ASR 模型
        self.asr_model = AutoModel(
            model=asr_model_dir,
            remote_code=remote_code_dir,
            trust_remote_code=True,
            device=device,
            disable_update=True
        )

    def run_vad(self, pcm_data, sample_rate):
        """
        运行 VAD 模型
        :param pcm_data: PCM 音频数据 (字节)
        :param sample_rate: 采样率
        :return: 是否仍在说话 (布尔值)
        """
        segments = self.vad_model(pcm_data, sample_rate)
        for segment in segments:
            start, end, _ = segment
            if end - start >= 240:  # VAD 推理的片段长度（毫秒）
                return True  # 说话仍在继续
        return False  # 说话中断

    def run_asr(self, pcm_data, sample_rate):
        """
        运行 ASR 模型
        :param pcm_data: PCM 音频数据 (字节)
        :param sample_rate: 采样率
        :return: 转录结果 (字符串)
        """
        transcription = self.asr_model(pcm_data, sample_rate)
        # 后处理转录结果
        processed_transcription = rich_transcription_postprocess(transcription)
        return processed_transcription
