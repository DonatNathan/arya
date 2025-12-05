#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <nlohmann/json.hpp>

#pragma once

struct Command {
    int code;
    std::string value;
};

class TranscriptAnalyzer {

    private:
        std::string& a_lastTranscript;
        std::vector<Command> a_commandList;

    public:
        TranscriptAnalyzer(std::string& lastTranscript);
        void loadCommands();
        std::string toLower(const std::string& s);
        int findCommand();

};
