#include <iostream>
#include <atomic>
#include <thread>
#include <SFML/Graphics.hpp>
#include "Globals.hpp"

#pragma once

class GraphicalInterface {
    private:
        std::string i_name;
        sf::RenderWindow i_window;
        sf::VideoMode i_mode;

        std::atomic<bool> i_shouldClose = true;
        std::atomic<bool> i_shouldOpen = true;
        std::atomic<bool> running = true;

    public:
        GraphicalInterface(std::string name, sf::VideoMode mode);
        ~GraphicalInterface();
        
        void createWindow();
        void open();
        void close();
        void clear();
        void checkEvents();
        void draw();
        void update();
};
