#include "Arya.hpp"

Arya::Arya() : a_recorder(a_audioBuffer, a_audioMutex), a_recognizer(a_audioBuffer, a_audioMutex, a_lastTranscript, a_recorder)
{
    if (!sf::SoundBufferRecorder::isAvailable()) {
        std::cerr << "No audio input available on your device." << std::endl;
        exit(1);
    }

    if (!a_recorder.start(44100)) {
        std::cerr << "Failed to start Arya's  ContinuousRecorder." << std::endl;
        exit(1);
    }
    
    while (true) {
        Update();
    }
};

Arya::~Arya()
{
    a_recorder.stop();
};

void Arya::CheckEvents()
{

};

void Arya::Update()
{
    if (!a_lastTranscript.empty()) {
        a_lastTranscript.clear();
    }
};
