#include <string>
#include <algorithm>
#include <vector>

#pragma once

class TranscriptAnalyzer {

    private:
        std::string& a_lastTranscript;

    public:
        TranscriptAnalyzer(std::string& lastTranscript);
        std::string toLower(const std::string& s);
        std::string analyzeTranscript();

};
