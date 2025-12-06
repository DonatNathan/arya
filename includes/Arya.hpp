#include <iostream>
#include <vector>

#include "ContinuousRecorder.hpp"
#include "SpeechRecognizer.hpp"
#include "TranscriptAnalyzer.hpp"
#include "ConversationEngine.hpp"

#pragma once

class Arya {

    private:
        std::vector<int16_t> a_audioBuffer;
        std::mutex a_audioMutex;
        std::string a_lastTranscript;

        ContinuousRecorder a_recorder;
        SpeechRecognizer a_recognizer;
        TranscriptAnalyzer a_analyzer;
        ConversationEngine a_cengine;
        
    public:
        Arya();
        ~Arya();
        void runArya();
        void checkEvents();
        void updateLoop();

};
