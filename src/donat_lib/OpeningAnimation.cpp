#include "donat_lib/OpeningAnimation.hpp"

OpeningAnimation::OpeningAnimation()
{

};

OpeningAnimation::~OpeningAnimation()
{

};

void OpeningAnimation::generateParticlesFromLetterA(sf::RenderWindow &window, unsigned int count)
{
    sf::Font font;
    if (!font.openFromFile("../assets/arial.ttf"))
        std::cerr << "Font file 'arial.ttf' not found." << std::endl;

    sf::RenderTexture rt({200, 200});
    rt.clear(sf::Color(0, 0, 0, 0));

    sf::Text text(font);
    text.setString("A");
    text.setCharacterSize(200);
    text.setFillColor(sf::Color::White);
    text.setPosition({0, 0});

    rt.draw(text);
    rt.display();

    sf::Image img = rt.getTexture().copyToImage();

    std::vector<sf::Vector2f> targets;
    for (unsigned y = 0; y < img.getSize().y; y++) {
        for (unsigned x = 0; x < img.getSize().x; x++) {
            sf::Color px = img.getPixel({x, y});
            if (px.a > 10 && (px.r + px.g + px.b) > 50) {
                targets.push_back({(float)x, (float)y});
            }
        }
    }

    i_particles.clear();
    i_particles.reserve(count);

    for (unsigned i = 0; i < count; i++) {
        sf::Vector2f t = targets[rand() % targets.size()];


        
        // center the letter in the window
        t.x += (window.getSize().x - img.getSize().x) / 2;
        t.y += (window.getSize().y - img.getSize().y) / 2;

        Particle p;
        p.target = t;
        p.position = {
            float(rand() % window.getSize().x),
            float(rand() % window.getSize().y)
        };

        p.velocity = {0.f, 0.f};
        p.color = sf::Color(0, 200, 255);

        i_particles.push_back(p);
    }
};

void OpeningAnimation::updateParticles(float dt)
{
    for (auto &p : i_particles) {
        sf::Vector2f dir = p.target - p.position;

        float distance = std::sqrt(dir.x*dir.x + dir.y*dir.y);

        if (distance > 0.1f)
            dir /= distance;

        float speed = std::clamp(distance * 3.f, 50.f, 500.f);

        p.velocity = dir * speed * dt;
        p.position += p.velocity;

        p.color.a = std::min<int>(255, 100 + rand() % 155);
    }
};

void OpeningAnimation::drawParticles(sf::RenderWindow &window)
{
    sf::CircleShape circ(2.f);
    circ.setOrigin({1.f, 1.f});
    for (auto &p : i_particles) {
        circ.setPosition(p.position);
        circ.setFillColor(p.color);
        window.draw(circ);
    }
};
