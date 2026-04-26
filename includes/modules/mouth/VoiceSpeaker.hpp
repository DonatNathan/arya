#include <string>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>

#pragma once

class VoiceSpeaker {
    private:
        std::string i_piperBin   = "../external/piper/piper";
        std::string i_voiceModel = "../external/piper/voices/libritts.onnx";

        std::queue<std::string>     i_queue;
        std::mutex                  i_mutex;
        std::condition_variable     i_cv;
        std::thread                 i_worker;
        std::atomic<bool>           i_running = true;

        void workerLoop();

    public:
        VoiceSpeaker();
        ~VoiceSpeaker();

        void say(const std::string& text);
        void setVoiceModel(const std::string& modelPath);
};