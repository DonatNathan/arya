#include <SFML/Graphics.hpp>

#pragma once

inline float WINDOW_WIDTH = 640;
inline float WINDOW_HEIGH = 480;

inline float FRAMERATE_LIMIT = 60;

inline const float FRAME = 1.0 / FRAMERATE_LIMIT;

const float SILENCE_THRESHOLD = 0.2f;
const float SPEECH_THRESHOLD  = 0.4f;

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
inline sf::Color EYES_WHITE = {255, 255, 255};
inline sf::Color EYES_YELLOW = {255, 255, 0};
inline sf::Color EYES_BLUE = {0, 0, 255};
inline sf::Color EYES_RED = {255, 0, 0};
