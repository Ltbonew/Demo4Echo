#ifndef AUDIOPROCESS_H
#define AUDIOPROCESS_H

#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <opus/opus.h>
#include <cstdint>
#include <thread>
#include <portaudio.h>

class AudioProcess {
public:
    // 构造函数，传入录音参数
    AudioProcess(int sample_rate = 16000, int channels = 1);
    ~AudioProcess();

    // 启动录音
    bool startRecording();

    // 停止录音
    bool stopRecording();

    // 清空录音队列
    void clearRecordedAudioQueue();

    /**
     * get recorded audio data from recordedAudioQueue.
     * 
     * @param recordedData The recorded audio data.
     * @return true if recorded audio data is available, false is empty.
     */
    bool getRecordedAudio(std::vector<int16_t>& recordedData);

    /**
     * Load audio data from a file and split it into frames.
     * 
     * @param filename The path to the audio file.
     * @param frame_duration_ms The duration of each frame in milliseconds.
     * @return A queue of audio frames.
     */
    std::queue<std::vector<int16_t>> loadAudioFromFile(const std::string& filename, int frame_duration_ms);

    /**
     * Save any queue of audio data to a PCM file.
     * 
     * @param filename The path to the PCM file.
     * @param audioQueue The queue containing audio data.
     */
    void saveToPCMFile(const std::string& filename, const std::queue<std::vector<int16_t>>& audioQueue);

    /**
     * Save recorded audio to a PCM file.
     * 
     * @param filename The path to the PCM file.
     */
    void saveToPCMFile(const std::string& filename);


    /**
     * Encode a PCM frame to Opus
     *
     * @param [in] pcm_frame The PCM frame to encode.
     * @param [out] opus_data The Opus data.
     * @param [out] opus_data_size The size of the Opus data.
     * @return true if encoding is successful, false otherwise.
     */
    bool encode(const std::vector<int16_t>& pcm_frame, uint8_t* opus_data, size_t& opus_data_size);

    /**
     * Decode Opus data to PCM frame
     * 
     * @param [in] opus_data The Opus data to decode.
     * @param [in] opus_data_size The size of the Opus data.
     * @param [out] pcm_frame The decoded PCM frame.
     * @return true if decoding is successful, false otherwise.
     */
    bool decode(const uint8_t* opus_data, size_t opus_data_size, std::vector<int16_t>& pcm_frame);


private:
    // PortAudio 回调函数
    static int recordCallback(const void *inputBuffer, void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo* timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void *userData);

    // Opus 编码器的状态
    OpusEncoder* encoder;
    // Opus 解码器
    OpusDecoder* decoder;

    // 采样率
    int sample_rate;
    // 通道数
    int channels;

    // 录音相关
    std::queue<std::vector<int16_t>> recordedAudioQueue;
    std::mutex recordedAudioMutex;
    std::condition_variable recordedAudioCV;
    bool isRecording;
    PaStream* stream;

    // 初始化编码器、解码器
    bool initializeOpus();
    // 释放 Opus 相关资源 编码器、解码器
    void cleanupOpus();
};

#endif // AUDIOPROCESS_H