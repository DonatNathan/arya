#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include "ContinuousRecorder.hpp"

#pragma once

class Arya {
    private:
        ContinuousRecorder m_rec;

    public:
        Arya();
       ~Arya();
       void CheckEvents();
       void Update();
};
