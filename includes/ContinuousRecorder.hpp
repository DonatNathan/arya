#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <mutex>

#pragma once

class ContinuousRecorder : public sf::SoundRecorder {
    private:
        std::vector<int16_t>& buffer;
        std::mutex& bufferMutex;

    protected:
        bool onStart() override;
        bool onProcessSamples(const std::int16_t* samples, std::size_t sampleCount) override;
        void onStop() override;

    public:
        ContinuousRecorder(std::vector<int16_t>& sharedBuffer, std::mutex& mtx);
};
