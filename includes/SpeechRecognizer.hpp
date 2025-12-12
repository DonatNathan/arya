#include <iostream>
#include <vector>
#include <mutex>
#include <cstring>
#include <thread>
#include <filesystem>

#include "whisper.h"
#include "ContinuousRecorder.hpp"

#pragma once

class SpeechRecognizer {

    private:
        std::vector<int16_t>& a_audioBuffer;
        std::mutex& a_audioMutex;
        std::string& a_lastTranscript;
        ContinuousRecorder& a_recorder;
        whisper_context* a_ctx;

    public:
        SpeechRecognizer(std::vector<int16_t>& sharedBuffer, std::mutex& mtx, std::string& lastTranscript, ContinuousRecorder& recorder, bool isDebugModeEnabled);
        void whisperLoop();
        std::vector<float> getAudioBuffer();
        whisper_full_params setupWhisper();
        std::vector<float> resampleTo16k(const std::vector<int16_t>& pcm44100);

};
