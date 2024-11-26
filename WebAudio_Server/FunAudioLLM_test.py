from funasr import AutoModel
from funasr.utils.postprocess_utils import rich_transcription_postprocess
import soundfile as sf

# 初始化模型
model_dir = "./FunAudioLLM/iic/SenseVoiceSmall"
vad_model_dir = "./FunAudioLLM/iic/speech_fsmn_vad_zh-cn-16k-common-pytorch"
remote_code_dir = "./FunAudioLLM/SenseVoice/model.py"

asr_model = AutoModel(
    model=model_dir,
    remote_code=remote_code_dir,
    trust_remote_code=True,
    device="cuda:0",
    disable_update=True
    )

vad_model = AutoModel(
    model=vad_model_dir,
    disable_pbar = True,
    max_end_silence_time=1000,
    vad_kwargs={"max_single_segment_time": 500},
    disable_update=True
    )

res = asr_model.generate(
    input=f"E:\projects\programs\python_codes\WebsocketAudio\FunAudioLLM\iic\SenseVoiceSmall\example\en.mp3",
    cache={},
    language="auto",  # "zh", "en", "yue", "ja", "ko", "nospeech"
    use_itn=True,
    batch_size_s=60,
    merge_vad=True,
    merge_length_s=15,
)
text = rich_transcription_postprocess(res[0]["text"])
print(text)


