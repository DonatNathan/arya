#include "graphical/GraphicalInterface.hpp"

GraphicalInterface::GraphicalInterface(std::string name, sf::VideoMode mode, std::string& lastTranscript) : a_lastTranscript(lastTranscript)
{
    i_name = name;
    i_mode = mode;
    i_areViewsCreated = false;
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
            } else {
                if (!i_areViewsCreated) {
                    sf::Vector2u size = i_window.getSize();

                    i_mainView.createView({(float)size.x * 0.5f, (float)size.y}, {{0.f, 0.f}, {0.5f, 1.f}}, {(float)size.x * 0.25f, (float)size.y * 0.5f}, size);
                    i_cameraView.createView({(float)size.x * 0.5f, (float)size.y * 0.5f}, {{0.5f, 0.f}, {0.5f, 0.5f}}, {(float)size.x * 0.75f, (float)size.y * 0.25f}, size);
                    i_terminalView.createView({(float)size.x * 0.5f, (float)size.y * 0.5f}, {{0.5f, 0.5f}, {0.5f, 0.5f}}, {(float)size.x * 0.75f, (float)size.y * 0.75f}, size);

                    i_areViewsCreated = true;
                }

                i_window.setView(i_cameraView.i_view);
                i_cameraView.drawView(i_window);

                i_window.setView(i_mainView.i_view);
                i_mainView.drawView(i_window);

                i_window.setView(i_terminalView.i_view);
                i_terminalView.drawView(i_window);
            }

            update();
            draw();
        }

        i_shouldOpen = false;
        i_shouldClose = false;
        i_animationDone = false;
        i_areViewsCreated = false;
    }
};

void GraphicalInterface::clear()
{
    i_window.clear(BACKGROUND_COLOR);
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