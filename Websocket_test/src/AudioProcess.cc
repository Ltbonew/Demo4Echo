#include "../inc/AudioProcess.h"
#include <iostream>
#include <fstream>


AudioProcess::AudioProcess(int sample_rate, int channels) : sample_rate(sample_rate), channels(channels), encoder(nullptr), decoder(nullptr), isRecording(false), stream(nullptr) {
    if (!initializeOpus()) {
        Log("Failed to initialize Opus encoder/decoder.", ERROR);
    }
}

AudioProcess::~AudioProcess() {
    cleanupOpus();
    if (isRecording) {
        stopRecording();
    }
}

void AudioProcess::Log(const std::string& message, LogLevel level) {
    std::string prefix = (level == ERROR) ? "[ERROR] " : "[INFO] ";
    std::cout << "[AudioProcess]" << prefix << message << std::endl;
}

bool AudioProcess::initializeOpus() {
    int error;

    // 初始化 Opus 编码器
    encoder = opus_encoder_create(sample_rate, channels, OPUS_APPLICATION_VOIP, &error);
    if (error != OPUS_OK) {
        Log("Opus encoder initialization failed: " + std::string(opus_strerror(error)), ERROR);
        return false;
    }

    // 初始化 Opus 解码器
    decoder = opus_decoder_create(sample_rate, channels, &error);
    if (error != OPUS_OK) {
        Log("Opus decoder initialization failed: " + std::string(opus_strerror(error)), ERROR);
        opus_encoder_destroy(encoder);
        return false;
    }
    return true;
}

void AudioProcess::cleanupOpus() {
    if (encoder) {
        opus_encoder_destroy(encoder);
    }
    if (decoder) {
        opus_decoder_destroy(decoder);
    }
}

bool AudioProcess::startRecording() {
    PaError err;

    // 初始化 PortAudio
    err = Pa_Initialize();
    if (err != paNoError) {
        Log("PortAudio error: " + std::string(Pa_GetErrorText(err)), ERROR);
        return false;
    }

    // 配置音频流参数
    PaStreamParameters inputParameters;
    inputParameters.device = Pa_GetDefaultInputDevice();
    if (inputParameters.device == paNoDevice) {
        Log("No default input device found.", ERROR);
        Pa_Terminate();
        return false;
    }
    inputParameters.channelCount = channels;       // 通道数
    inputParameters.sampleFormat = paInt16;       // 16 位样本
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = nullptr;

    // 打开音频流
    err = Pa_OpenStream(&stream,
                        &inputParameters,
                        nullptr, // 无输出
                        sample_rate,
                        1600, // 每缓冲区的帧数
                        paClipOff, // 不剪裁样本
                        recordCallback,
                        this);
    if (err != paNoError) {
        Log("Error opening stream: " + std::string(Pa_GetErrorText(err)), ERROR);
        Pa_Terminate();
        return false;
    }

    // 开始录制
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        Log("Error starting stream: " + std::string(Pa_GetErrorText(err)), ERROR);
        Pa_CloseStream(stream);
        Pa_Terminate();
        return false;
    }

    isRecording = true;
    Log("Recording started.", INFO);
    return true;
}

void AudioProcess::stopRecording() {
    PaError err;

    // 停止录制
    err = Pa_StopStream(stream);
    if (err != paNoError) {
        Log("Error stopping stream: " + std::string(Pa_GetErrorText(err)), ERROR);
    }

    // 关闭音频流
    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        Log("Error closing stream: " + std::string(Pa_GetErrorText(err)), ERROR);
    }

    // 释放 PortAudio 资源
    Pa_Terminate();

    isRecording = false;
    Log("Recording stopped.", INFO);
}

bool AudioProcess::getRecordedAudio(std::vector<int16_t>& recordedData) {
    std::unique_lock<std::mutex> lock(recordedAudioMutex);
    recordedAudioCV.wait(lock, [this] { return !recordedAudioQueue.empty() || !isRecording; });

    if (recordedAudioQueue.empty()) {
        return false; // 队列为空且不再录音
    }

    recordedData.swap(recordedAudioQueue.front());
    recordedAudioQueue.pop();
    return true;
}

int AudioProcess::recordCallback(const void *inputBuffer, void *outputBuffer,
                                 unsigned long framesPerBuffer,
                                 const PaStreamCallbackTimeInfo* timeInfo,
                                 PaStreamCallbackFlags statusFlags,
                                 void *userData) {
    (void) outputBuffer; // 未使用输出缓冲区
    (void) timeInfo;     // 未使用时间信息
    (void) statusFlags;  // 未使用状态标志

    AudioProcess* audioProcess = static_cast<AudioProcess*>(userData);
    const int16_t* input = static_cast<const int16_t*>(inputBuffer);

    std::vector<int16_t> frame(framesPerBuffer);
    std::copy(input, input + framesPerBuffer, frame.begin());

    {
        std::lock_guard<std::mutex> lock(audioProcess->recordedAudioMutex);
        audioProcess->recordedAudioQueue.push(frame);
    }
    audioProcess->recordedAudioCV.notify_one();

    return paContinue;
}

std::queue<std::vector<int16_t>> AudioProcess::loadAudioFromFile(const std::string& filename, int frame_duration_ms) {
    std::ifstream infile(filename, std::ios::binary);
    if (!infile) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return {};
    }

    // 获取文件大小
    infile.seekg(0, std::ios::end);
    std::streampos fileSize = infile.tellg();
    infile.seekg(0, std::ios::beg);

    // 计算样本数量
    size_t numSamples = static_cast<size_t>(fileSize) / sizeof(int16_t);

    // 读取音频数据
    std::vector<int16_t> audio_data(numSamples);
    infile.read(reinterpret_cast<char*>(audio_data.data()), fileSize);

    if (!infile) {
        std::cerr << "Error reading file: " << filename << std::endl;
        return {};
    }

    // 计算每帧的样本数量
    int frame_size = sample_rate / 1000 * frame_duration_ms;

    // 将音频数据切分成帧
    std::queue<std::vector<int16_t>> audio_frames;
    for (size_t i = 0; i < numSamples; i += frame_size) {
        size_t remaining_samples = numSamples - i;
        size_t current_frame_size = (remaining_samples > frame_size) ? frame_size : remaining_samples;

        std::vector<int16_t> frame(current_frame_size);
        std::copy(audio_data.begin() + i, audio_data.begin() + i + current_frame_size, frame.begin());
        audio_frames.push(frame);
    }

    return audio_frames;
}


void AudioProcess::saveToPCMFile(const std::string& filename, const std::queue<std::vector<int16_t>>& audioQueue) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        Log("Failed to open file: " + filename, ERROR);
        return;
    }

    {
        std::queue<std::vector<int16_t>> tempQueue = audioQueue;
        while (!tempQueue.empty()) {
            const std::vector<int16_t>& frame = tempQueue.front();
            file.write(reinterpret_cast<const char*>(frame.data()), frame.size() * sizeof(int16_t));
            tempQueue.pop();
        }
    }

    file.close();
    Log("Saved recording to " + filename, INFO);
}

void AudioProcess::saveToPCMFile(const std::string& filename) {
    std::unique_lock<std::mutex> lock(recordedAudioMutex);
    saveToPCMFile(filename, recordedAudioQueue);
}

bool AudioProcess::encode(const std::vector<int16_t>& pcm_frame, uint8_t* opus_data, size_t& opus_data_size) {
    if (!encoder) {
        Log("Encoder not initialized", ERROR);
        return false;
    }

    int frame_size = pcm_frame.size();

    if (frame_size <= 0) {
        Log("Invalid PCM frame size: " + std::to_string(frame_size), ERROR);
        return false;
    }

    // 对当前帧进行编码
    int encoded_bytes_size = opus_encode(encoder, pcm_frame.data(), frame_size, opus_data, 2048); // max 2048 bytes

    if (encoded_bytes_size < 0) {
        Log("Encoding failed: " + std::string(opus_strerror(encoded_bytes_size)), ERROR);
        return false;
    }

    opus_data_size = static_cast<size_t>(encoded_bytes_size);
    return true;
}

bool AudioProcess::decode(const uint8_t* opus_data, size_t opus_data_size, std::vector<int16_t>& pcm_frame) {
    if (!decoder) {
        Log("Decoder not initialized", ERROR);
        return false;
    }

    int frame_size = 960;  // 20ms 帧, 16000Hz 采样率, 理论上应该是 320 个样本，但是 Opus 限制为 960
    pcm_frame.resize(frame_size * channels);

    // 对当前帧进行解码
    int decoded_samples = opus_decode(decoder, opus_data, static_cast<int>(opus_data_size), pcm_frame.data(), frame_size, 0);

    if (decoded_samples < 0) {
        Log("Decoding failed: " + std::string(opus_strerror(decoded_samples)), ERROR);
        return false;
    }

    pcm_frame.resize(decoded_samples * channels);
    return true;
}

BinProtocol* AudioProcess::PackBinFrame(const uint8_t* payload, size_t payload_size) {
    // Allocate memory for BinaryProtocol + payload
    auto pack = (BinProtocol*)malloc(sizeof(BinProtocol) + payload_size);
    if (!pack) {
        std::cerr << "Memory allocation failed" << std::endl;
        return nullptr;
    }

    pack->version = htons(1);
    pack->type = htons(0);  // Indicate audio data type
    pack->payload_size = htonl(payload_size);
    assert(sizeof(BinProtocol) == 8);

    // Copy payload data
    memcpy(pack->payload, payload, payload_size);

    return pack;
}

bool AudioProcess::UnpackBinFrame(const uint8_t* packed_data, size_t packed_data_size, BinProtocol& unpacked_frame) {
    // 检查输入数据的有效性
    if (packed_data_size < sizeof(BinProtocol) - sizeof(uint8_t)) {
        Log("Packed data size is too small", ERROR);
        return false;
    }

    // 计算 payload 的大小
    const BinProtocol* bin_frame = reinterpret_cast<const BinProtocol*>(packed_data);
    size_t payload_size = ntohl(bin_frame->payload_size);

    // 检查总数据大小是否匹配
    if (packed_data_size < sizeof(BinProtocol) - sizeof(uint8_t) + payload_size) {
        Log("Packed data size does not match payload size", ERROR);
        return false;
    }

    // 计算总大小
    size_t total_size = sizeof(BinProtocol) - sizeof(uint8_t) + payload_size;

    // 动态分配内存
    unpacked_frame = *reinterpret_cast<const BinProtocol*>(new uint8_t[total_size]);
    unpacked_frame.version = ntohs(bin_frame->version);
    unpacked_frame.type = ntohs(bin_frame->type);
    unpacked_frame.payload_size = payload_size;
    std::copy(bin_frame->payload, bin_frame->payload + payload_size, unpacked_frame.payload);

    return true;
}