#include "graphical/GraphicalInterface.hpp"

GraphicalInterface::GraphicalInterface(std::string name, sf::VideoMode mode, std::string& lastTranscript) : a_lastTranscript(lastTranscript)
{
    i_name = name;
    i_mode = mode;
};

GraphicalInterface::~GraphicalInterface()
{

};

void GraphicalInterface::createWindow()
{
    while (running) {
        
        while (!i_shouldOpen && running)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (!running)
            break;

        i_window.create(i_mode, i_name, sf::State::Fullscreen);
        i_window.setFramerateLimit(FRAMERATE_LIMIT);
        i_window.setMouseCursorVisible(false);

        i_openingAnimation.generateParticlesFromLetterA(i_window);

        while (i_window.isOpen()) {

            if (i_shouldClose) {
                i_window.close();
                break;
            }

            clear();
            checkEvents();

            float dt = i_clock.restart().asSeconds();

            if (!i_animationDone) {
                i_openingAnimation.updateParticles(dt);
                i_openingAnimation.drawParticles(i_window);
                if (i_openingAnimation.isAnimationFinished())
                    i_animationDone = true;
            }

            update();
            draw();
        }

        i_shouldOpen = false;
        i_shouldClose = false;
        i_animationDone = false;
    }
};

void GraphicalInterface::clear()
{
    i_window.clear(sf::Color::Black);
};

void GraphicalInterface::checkEvents()
{
    while (const std::optional event = i_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            i_window.close();
        } else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
            if (keyReleased->scancode == sf::Keyboard::Scancode::Escape)
                i_window.close();
        }
    }
};

void GraphicalInterface::update()
{
    
};

void GraphicalInterface::draw()
{
    i_window.display();
};

void GraphicalInterface::open()
{
    i_shouldOpen = true;
};

void GraphicalInterface::close()
{
    i_shouldClose = true;
};