#include "graphical/OpeningAnimation.hpp"

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
        std::cerr << getColorFromCode(Color::RED) << "Font file 'arial.ttf' not found." << getColorFromCode(Color::RESET) << std::endl;

    sf::RenderTexture rt({200, 200});
    rt.clear(sf::Color(0, 0, 0, 0));

    sf::Text text(font);
    text.setString("A");
    text.setCharacterSize(200);
    text.setFillColor(sf::Color::White);
    text.setPosition({0, 0});

    sf::FloatRect bounds = text.getLocalBounds();
    sf::Vector2f textSize = bounds.size;
    sf::Vector2f textPosition = bounds.position;
    text.setOrigin({textPosition.x + textSize.x / 2.f, textPosition.y + textSize.y/ 2.f});
    text.setPosition({rt.getSize().x / 2.f, rt.getSize().y / 2.f});

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

        t.x += (window.getSize().x - img.getSize().x) / 2;
        t.y += (window.getSize().y - img.getSize().y) / 2;

        Particle p;
        p.target = t;
        sf::Vector2f center(window.getSize().x / 2.f, window.getSize().y / 2.f);
        float maxOffset = 150.f;

        p.position = {
            center.x + ((rand() % 2000) / 1000.f - 1.f) * maxOffset,
            center.y + ((rand() % 2000) / 1000.f - 1.f) * maxOffset
        };

        p.velocity = {0.f, 0.f};
        p.color = sf::Color(MAIN_COLOR);

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

bool OpeningAnimation::isAnimationFinished()
{
    float arrivalThreshold = 2.f;

    for (const auto& p : i_particles) {
        sf::Vector2f diff = p.target - p.position;
        float distance = std::sqrt(diff.x*diff.x + diff.y*diff.y);

        if (distance > arrivalThreshold)
            return false;
    }

    return true;
};
