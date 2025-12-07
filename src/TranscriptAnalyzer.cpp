#include "TranscriptAnalyzer.hpp"

TranscriptAnalyzer::TranscriptAnalyzer(std::string& lastTranscript) : a_lastTranscript(lastTranscript)
{
    
};

std::string TranscriptAnalyzer::toLower(const std::string& s)
{
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(), ::tolower);
    return out;
};

std::string TranscriptAnalyzer::analyzeTranscript()
{
    std::string t = toLower(a_lastTranscript);

    return t;
};
