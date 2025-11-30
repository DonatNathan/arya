#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
#include <cstring>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include "ContinuousRecorder.hpp"
#include "whisper.h"

#pragma once

class Arya {
    private:
        std::vector<int16_t> m_audioBuffer;
        std::mutex m_audioMutex;
        std::string lastTranscript;
        ContinuousRecorder m_rec;
        
    public:
        Arya();
        ~Arya();
        void CheckEvents();
        void Update();
        void WhisperLoop();
        std::vector<float> resampleTo16k(const std::vector<int16_t>& pcm44100);
};
