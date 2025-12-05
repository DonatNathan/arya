#include "TranscriptAnalyzer.hpp"

TranscriptAnalyzer::TranscriptAnalyzer(std::string& lastTranscript) : a_lastTranscript(lastTranscript)
{
    loadCommands();
};

void TranscriptAnalyzer::loadCommands()
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

std::string TranscriptAnalyzer::toLower(const std::string& s)
{
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(), ::tolower);
    return out;
};

int TranscriptAnalyzer::findCommand()
{
    std::string t = toLower(a_lastTranscript);

    for (const auto& cmd : a_commandList) {
        std::string valueLower = toLower(cmd.value);

        if (t.find(valueLower) != std::string::npos) {
            return cmd.code;
        }
    }

    return 0;
};
