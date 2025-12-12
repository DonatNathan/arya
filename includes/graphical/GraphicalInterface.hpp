#include <iostream>
#include <atomic>
#include <thread>
#include <SFML/Graphics.hpp>
#include "Globals.hpp"

#include "graphical/OpeningAnimation.hpp"

#pragma once

class GraphicalInterface {
    private:
        std::string i_name;
        sf::RenderWindow i_window;
        sf::VideoMode i_mode;
        sf::Clock i_clock;
        std::string& a_lastTranscript;
        bool i_animationDone = false;

        std::atomic<bool> i_shouldClose = true;
        std::atomic<bool> i_shouldOpen = true;
        std::atomic<bool> running = true;

        OpeningAnimation i_openingAnimation;

    public:
        GraphicalInterface(std::string name, sf::VideoMode mode, std::string& lastTranscript);
        ~GraphicalInterface();
        
        void createWindow();
        void open();
        void close();
        void clear();
        void checkEvents();
        void draw();
        void update();
};
