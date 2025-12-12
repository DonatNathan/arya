#include "ContinuousRecorder.hpp"

ContinuousRecorder::ContinuousRecorder(std::vector<int16_t>& sharedBuffer, std::mutex& mtx) : buffer(sharedBuffer), bufferMutex(mtx)
{

};

bool ContinuousRecorder::onStart()
{
    std::cout << getColorFromCode(Color::GREEN) << "ContinuousRecorder running..." << getColorFromCode(Color::RESET) << std::endl;
    return true;
};

bool ContinuousRecorder::onProcessSamples(const int16_t* samples, std::size_t count)
{
    static bool inSpeech = false;
    static float silenceTime = 0.f;

    {
        std::lock_guard<std::mutex> lock(bufferMutex);
        buffer.insert(buffer.end(), samples, samples + count);
    }

    float rms = computeRMS(samples, count);
    double chunkDuration = double(count) / 44100.0;

    if (rms > SPEECH_THRESHOLD) {
        inSpeech = true;
        silenceTime = 0;
    } else if (rms < SILENCE_THRESHOLD && inSpeech) {
        silenceTime += chunkDuration;

        if (silenceTime >= 0.5) {
            inSpeech = false;

            {
                std::lock_guard<std::mutex> lock(bufferMutex);
                readyToTranscribe = true;
            }
        }
    }

    return true;
}

void ContinuousRecorder::onStop()
{
    std::cout << getColorFromCode(Color::GREEN) << "ContinuousRecorder stopped." << getColorFromCode(Color::RESET) << std::endl;
};

float ContinuousRecorder::computeRMS(const int16_t* samples, size_t count)
{
    double sum = 0.0;
    for (size_t i = 0; i < count; i++) {
        sum += samples[i] * samples[i];
    }
    return std::sqrt(sum / count) / 32768.0f;
};
