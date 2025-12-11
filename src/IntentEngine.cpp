#include "IntentEngine.hpp"

IntentEngine::IntentEngine()
{
    loadCommands();
};

IntentEngine::~IntentEngine()
{

};

void IntentEngine::loadCommands()
{
    std::string path = "../assets/commands.json";
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open command file: " + path);
    }

    nlohmann::json j;
    file >> j;

    for (auto& entry : j) {
        a_commandList.push_back({
            entry["code"].get<int>(),
            entry["value"].get<std::string>()
        });
    }
};

int IntentEngine::findCommand(std::string analyzedTranscript)
{
    for (const auto& cmd : a_commandList) {
        std::string value = cmd.value;
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);

        if (analyzedTranscript.find(value) != std::string::npos) {
            return cmd.code;
        }
    }

    return 0;
};

Intent IntentEngine::selectCommand(std::string analyzedTranscript)
{
    int code = findCommand(analyzedTranscript);

    switch (code) {
        case 0:
            return Intent::NONE;
        case 1:
            return Intent::CORRECTION_MODE;
        case 2:
            return Intent::OPEN_INTERFACE;
        case 3:
            return Intent::CLOSE_INTERFACE;
        case 4:
            return Intent::CAMERA_ON;
        case 5:
            return Intent::CAMERA_OFF;
        case 6:
            return Intent::TURN_OFF;
        default:
            return Intent::UNKNOWN;
    }
};
