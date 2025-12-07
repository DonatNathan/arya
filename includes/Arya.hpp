#include <iostream>
#include <vector>

#include "ContinuousRecorder.hpp"
#include "SpeechRecognizer.hpp"
#include "TranscriptAnalyzer.hpp"
#include "IntentEngine.hpp"

#pragma once

class Arya {

    private:
        std::vector<int16_t> a_audioBuffer;
        std::mutex a_audioMutex;
        std::string a_lastTranscript;

        ContinuousRecorder a_recorder;
        SpeechRecognizer a_recognizer;
        TranscriptAnalyzer a_analyzer;
        IntentEngine a_iengine;
        
    public:
        Arya();
        ~Arya();
        void runArya();
        void checkEvents();
        void updateLoop();
        std::string executeCommand(Intent cmd);

};
