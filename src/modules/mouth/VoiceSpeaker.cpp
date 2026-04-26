#include "modules/mouth/VoiceSpeaker.hpp"
#include <iostream>
#include <cstdlib>

VoiceSpeaker::VoiceSpeaker()
{
    i_worker = std::thread(&VoiceSpeaker::workerLoop, this);
};

VoiceSpeaker::~VoiceSpeaker()
{
    i_running = false;
    i_cv.notify_all();
    if (i_worker.joinable())
        i_worker.join();
};

void VoiceSpeaker::say(const std::string& text)
{
    {
        std::lock_guard<std::mutex> lock(i_mutex);
        i_queue.push(text);
    }
    i_cv.notify_one();
};

void VoiceSpeaker::workerLoop()
{
    while (i_running) {
        std::unique_lock<std::mutex> lock(i_mutex);
        i_cv.wait(lock, [this] { return !i_queue.empty() || !i_running; });

        if (!i_running) break;

        std::string text = i_queue.front();
        i_queue.pop();
        lock.unlock();

        std::string cmd = "echo '" + text + "' | " +
                  i_piperBin + " --model " + i_voiceModel +
                  " --output-raw 2>/dev/null | aplay -r 22050 -f S16_LE -c 1 -q";

        std::system(cmd.c_str());
    }
};

void VoiceSpeaker::setVoiceModel(const std::string& modelPath)
{
    i_voiceModel = modelPath;
};