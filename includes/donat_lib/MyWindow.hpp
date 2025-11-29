#include <iostream>
#include <SFML/Graphics.hpp>
#include "Globals.hpp"

#pragma once

class MyWindow {
    private:
        std::string m_name;
        sf::RenderWindow m_window;
        sf::VideoMode m_mode;

    public:
        MyWindow(std::string name, sf::VideoMode mode);
        ~MyWindow();
        
        void Init();
        void Clear();
        void CheckEvents();
        void Draw();
        void Update();
};
