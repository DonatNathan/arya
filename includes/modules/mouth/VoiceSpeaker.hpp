#include <espeak-ng/speak_lib.h>
#include <string>
#include <iostream>
#include <thread>

#include "Globals.hpp"
#include "Utils.hpp"

#pragma once

class VoiceSpeaker {
    private:
        int i_sampleRate = 22050;
        int i_volume     = 100;
        int i_speed      = 150;
        int i_pitch      = 50;

    public:
        VoiceSpeaker();
        ~VoiceSpeaker();

        void say(const std::string& text);
        void setVoice(const std::string& voice);
        void setVolume(int volume);
        void setSpeed(int speed);
        void setPitch(int pitch);
};
