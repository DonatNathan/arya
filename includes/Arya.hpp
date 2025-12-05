#include <iostream>
#include <vector>

#include "ContinuousRecorder.hpp"
#include "SpeechRecognizer.hpp"

#pragma once

class Arya {

    private:
        std::vector<int16_t> a_audioBuffer;
        std::mutex a_audioMutex;
        std::string a_lastTranscript;

        ContinuousRecorder a_recorder;
        SpeechRecognizer a_recognizer;
        
    public:
        Arya();
        ~Arya();
        void CheckEvents();
        void Update();

};
