#include "graphical/ViewInterface.hpp"

ViewInterface::ViewInterface(float tickness)
{
    i_tickness = tickness;
};

ViewInterface::~ViewInterface()
{

};

void ViewInterface::createView(sf::Vector2f viewSize, sf::FloatRect viewPosition, sf::Vector2f viewCenter, sf::Vector2u windowSize)
{
    i_view.setSize(viewSize);
    i_view.setCenter(viewCenter);
    i_view.setViewport(viewPosition);

    i_viewShape.setSize({viewSize.x - 2 * i_tickness, viewSize.y - 2 * i_tickness});
    i_viewShape.setPosition({viewPosition.position.x * windowSize.x + i_tickness, viewPosition.position.y * windowSize.y + i_tickness});
    i_viewShape.setFillColor(sf::Color::Transparent);
    i_viewShape.setOutlineThickness(i_tickness);
    i_viewShape.setOutlineColor(MAIN_COLOR);
};

void ViewInterface::drawView(sf::RenderWindow& window)
{
    window.draw(i_viewShape);
};
