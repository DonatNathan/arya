#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>

#pragma once

class ContinuousRecorder : public sf::SoundRecorder {
    bool onStart() override
    {
        std::cout << "Recording started\n";
        return true;
    }

    bool onProcessSamples(const std::int16_t* samples, std::size_t sampleCount) override
    {
        std::cout << "Received " << sampleCount << " samples\n";

        // ---- Example use cases ----
        // 1. Push samples into a queue for a background thread
        // 2. Send samples over websocket to a speech API
        // 3. Run DSP / VAD / hotword detection
        // 4. Save a rolling buffer for Whisper.cpp

        // buffer.insert(buffer.end(), samples, samples + sampleCount);

        return true;
    }

    void onStop() override
    {
        std::cout << "Recording stopped\n";
    }
};
