#include "Arya.hpp"

Arya::Arya(bool isDebugModeEnabled) : a_recorder(a_audioBuffer, a_audioMutex), a_recognizer(a_audioBuffer, a_audioMutex, a_aryaMutex, a_waitForTranscript, a_lastTranscript, a_recorder, isDebugModeEnabled), a_analyzer(a_lastTranscript), a_interface("Arya", sf::VideoMode::getDesktopMode(), a_lastTranscript)
{
    a_isDebugModeEnabled = isDebugModeEnabled;

    if (!a_isDebugModeEnabled) {
        if (!sf::SoundBufferRecorder::isAvailable()) {
            std::cerr << getColorFromCode(Color::RED) << "No audio input available on your device." << getColorFromCode(Color::RESET) << std::endl;
            exit(1);
        }
    
        if (!a_recorder.start(44100)) {
            std::cerr << getColorFromCode(Color::RED) << "Failed to start Arya's  ContinuousRecorder." << getColorFromCode(Color::RESET) << std::endl;
            exit(1);
        }
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
        if (a_isDebugModeEnabled) {
            std::string adminInput;
            std::cout << getColorFromCode(Color::YELLOW) << "ADMIN> " << getColorFromCode(Color::RESET);
            std::getline(std::cin, adminInput);
            {
                std::lock_guard<std::mutex> lock(a_aryaMutex);
                a_lastTranscript = adminInput;
            }
            a_waitForTranscript.notify_one();
        }
        std::unique_lock<std::mutex> lock(a_aryaMutex);
        a_waitForTranscript.wait(lock, [&]{ return !a_lastTranscript.empty(); });
        updateLoop();
    }
};

void Arya::checkEvents()
{

};

void Arya::updateLoop()
{
    std::string response;
    std::cout << "[TRANSCRIPT] " << a_lastTranscript << "\n";

    std::string analyzedTranscript = a_analyzer.analyzeTranscript();

    Intent command = a_iengine.selectCommand(analyzedTranscript);

    if (command != Intent::NONE)
        response = executeCommand(command);

    std::cout << "Response: " << response << std::endl;
    a_lastTranscript.clear();
};

std::string Arya::executeCommand(Intent cmd)
{
    switch (cmd) {
        case Intent::CORRECTION_MODE:
            return "Correction mode enabled.";
        case Intent::OPEN_INTERFACE:
            a_interface.open();
            return "Graphical interface opened.";
        case Intent::CLOSE_INTERFACE:
            a_interface.close();
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
