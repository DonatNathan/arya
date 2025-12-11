#include <iostream>
#include <vector>

#include "donat_lib/GraphicalInterface.hpp"

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

        std::thread a_uiThread;
        GraphicalInterface a_interface;

        ContinuousRecorder a_recorder;
        SpeechRecognizer a_recognizer;
        TranscriptAnalyzer a_analyzer;
        IntentEngine a_iengine;

    public:
        Arya();
        ~Arya();

        void openGraphicalInterface();
        void closeGraphicalInterface();

        void runArya();
        void checkEvents();
        void updateLoop();

        std::string executeCommand(Intent cmd);

};
