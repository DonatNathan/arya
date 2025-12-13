#include <SFML/Graphics.hpp>

#include "Globals.hpp"

#pragma once

class ViewInterface {
    private:
        sf::RectangleShape i_viewShape;
        float i_tickness;
    
    public:
        sf::View i_view;
        ViewInterface(float tickness = 5);
        ~ViewInterface();
        void createView(sf::Vector2f viewSize, sf::FloatRect viewPosition, sf::Vector2f viewCenter, sf::Vector2u windowSize);
        void drawView(sf::RenderWindow& window);

};
