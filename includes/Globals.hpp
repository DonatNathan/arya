#include <SFML/Graphics.hpp>

#pragma once

inline float WINDOW_WIDTH = 640;
inline float WINDOW_HEIGH = 480;

inline float FRAMERATE_LIMIT = 60;

inline const float FRAME = 1.0 / FRAMERATE_LIMIT;

const float SILENCE_THRESHOLD = 0.001f;
const float SPEECH_THRESHOLD  = 0.005f;

// Colors

enum class Color {
    RESET = 0,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34
};

// Graphical Interface

inline float PARTICLES_COUNT = 1000;

inline sf::Color MAIN_COLOR = {255, 59, 59};
inline sf::Color BACKGROUND_COLOR = {10, 15, 24};
