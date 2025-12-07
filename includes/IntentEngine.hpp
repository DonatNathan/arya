#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

#pragma once 

struct Command {
    int code;
    std::string value;
};

enum class Intent {
    NONE = 0,
    CORRECTION_MODE = 1,
    OPEN_INTERFACE = 2,
    CLOSE_INTERFACE = 3,
    CAMERA_ON = 4,
    CAMERA_OFF = 5,
    UNKNOWN = -1
};

class IntentEngine {

    private:
        std::vector<Command> a_commandList;

    public:
        IntentEngine();
        ~IntentEngine();
        void loadCommands();
        int findCommand(std::string analyzedTranscript);
        Intent selectCommand(std::string analyzedTranscript);

};
