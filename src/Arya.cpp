#include "Arya.hpp"

Arya::Arya() : a_recorder(a_audioBuffer, a_audioMutex), a_recognizer(a_audioBuffer, a_audioMutex, a_lastTranscript, a_recorder), a_analyzer(a_lastTranscript), a_interface("Arya", sf::VideoMode::getDesktopMode(), a_lastTranscript)
{
    if (!sf::SoundBufferRecorder::isAvailable()) {
        std::cerr << "No audio input available on your device." << std::endl;
        exit(1);
    }

    if (!a_recorder.start(44100)) {
        std::cerr << "Failed to start Arya's  ContinuousRecorder." << std::endl;
        exit(1);
    }

    a_uiThread = std::thread(&GraphicalInterface::createWindow, &a_interface);
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
        std::string response;
        std::cout << "[TRANSCRIPT] " << a_lastTranscript << "\n";

        std::string analyzedTranscript = a_analyzer.analyzeTranscript();
        Intent command = a_iengine.selectCommand(analyzedTranscript);

        if (command != Intent::NONE)
            response = executeCommand(command);
        
        std::cout << "Response: " << response << std::endl;
        a_lastTranscript.clear();
    }
};

std::string Arya::executeCommand(Intent cmd)
{
    switch (cmd) {
        case Intent::CORRECTION_MODE:
            return "Correction mode enabled.";
        case Intent::OPEN_INTERFACE:
            openGraphicalInterface();
            return "Graphical interface opened.";
        case Intent::CLOSE_INTERFACE:
            closeGraphicalInterface();
            return "Graphical interface closed.";
        case Intent::CAMERA_ON:
            return "Camera turned on.";
        case Intent::CAMERA_OFF:
            return "Camera turned off.";
        case Intent::TURN_OFF:
            exit(0);
        default:
            return "Invalid command.";
    }
};

void Arya::openGraphicalInterface()
{
    a_interface.open();
};

void Arya::closeGraphicalInterface()
{
    a_interface.close();
};
