#include "Arya.hpp"

Arya::Arya()
{
    if (!sf::SoundBufferRecorder::isAvailable()) {
        std::cerr << "No audio input available on your device" << std::endl;
    }

    if (!m_rec.start())
        std::cerr << "Failed to start Arya's continuous recorder" << std::endl;
};

Arya::~Arya()
{
    m_rec.stop();
};

void Arya::CheckEvents()
{

};

void Arya::Update()
{
    
};
