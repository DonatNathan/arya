#include "Arya.hpp"

Arya::Arya() : a_recorder(a_audioBuffer, a_audioMutex), a_recognizer(a_audioBuffer, a_audioMutex, a_lastTranscript, a_recorder), a_analyzer(a_lastTranscript)
{
    if (!sf::SoundBufferRecorder::isAvailable()) {
        std::cerr << "No audio input available on your device." << std::endl;
        exit(1);
    }

    if (!a_recorder.start(44100)) {
        std::cerr << "Failed to start Arya's  ContinuousRecorder." << std::endl;
        exit(1);
    }
};

Arya::~Arya()
{
    a_recorder.stop();
};

void Arya::runArya()
{
    while (true) {
        updateLoop();
    }
};

void Arya::checkEvents()
{

};

void Arya::updateLoop()
{
    if (!a_lastTranscript.empty()) {
        std::cout << "[TRANSCRIPT] " << a_lastTranscript << "\n";
        int code = a_analyzer.findCommand();
        std::cout << "Code: " << code << std::endl;
        a_lastTranscript.clear();
    }
};
