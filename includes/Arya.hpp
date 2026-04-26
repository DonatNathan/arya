#include <iostream>
#include <vector>
#include <condition_variable>

#include "Utils.hpp"

#include "graphical/GraphicalInterface.hpp"

#include "modules/ears/ContinuousRecorder.hpp"
#include "modules/ears/SpeechRecognizer.hpp"
#include "modules/brain/TranscriptAnalyzer.hpp"
#include "modules/brain/IntentEngine.hpp"
#include "modules/mouth/VoiceSpeaker.hpp"

#pragma once

class Arya {

    private:
        std::vector<int16_t> a_audioBuffer;
        std::mutex a_audioMutex;
        std::string a_lastTranscript;
        bool a_isDebugModeEnabled;

        std::mutex a_aryaMutex;
        std::condition_variable a_waitForTranscript;

        std::thread a_uiThread;
        GraphicalInterface a_interface;

        ContinuousRecorder a_recorder;
        SpeechRecognizer a_recognizer;
        TranscriptAnalyzer a_analyzer;
        IntentEngine a_iengine;
        VoiceSpeaker a_speaker;

    public:
        Arya(bool isDebugModeEnabled);
        ~Arya();

        void runArya();
        void checkEvents();
        void updateLoop();

        std::string executeCommand(Intent cmd);

};
