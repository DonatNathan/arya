#include "modules/mouth/VoiceSpeaker.hpp"

VoiceSpeaker::VoiceSpeaker()
{
    if (espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 0, nullptr, 0) < 0)
        std::cerr << "[VoiceSpeaker] Failed to initialize espeak-ng\n";
    
    // setVoice("en-us-female");
    setVoice("mb-us1");
};

VoiceSpeaker::~VoiceSpeaker()
{
    espeak_Terminate();
};

void VoiceSpeaker::say(const std::string& text)
{
    std::thread([this, text]() {
        espeak_SetParameter(espeakVOLUME, i_volume, 0);
        espeak_SetParameter(espeakRATE,   i_speed,  0);
        espeak_SetParameter(espeakPITCH,  i_pitch,  0);
    
        espeak_Synth(
            text.c_str(),
            text.size() + 1,
            0,
            POS_CHARACTER,
            0,
            espeakCHARS_AUTO,
            nullptr,
            nullptr
        );
    
        espeak_Synchronize();
    }).detach();
};

void VoiceSpeaker::setVoice(const std::string& voice)
{
    if (espeak_SetVoiceByName(voice.c_str()) != EE_OK)
        std::cerr << getColorFromCode(Color::RED) << "[VoiceSpeaker] Failed to set voice: " << voice << getColorFromCode(Color::RESET) << "\n";
};

void VoiceSpeaker::setVolume(int volume)
{
    i_volume = volume;
};

void VoiceSpeaker::setSpeed(int speed)
{
    i_speed  = speed;
};

void VoiceSpeaker::setPitch(int pitch)
{
    i_pitch  = pitch;
};
