#include <vector>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <SFML/Graphics.hpp>

#pragma once

struct Particle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f target;
    sf::Color color;
};

class OpeningAnimation {
    private:
        std::vector<Particle> i_particles;

    public:
        OpeningAnimation();
        ~OpeningAnimation();

        void generateParticlesFromLetterA(sf::RenderWindow &window, unsigned int count = 2000);
        void updateParticles(float dt);
        void drawParticles(sf::RenderWindow &window);

};
