#include "donat_lib/MyWindow.hpp"

MyWindow::MyWindow(std::string name, sf::VideoMode mode)
{
    m_name = name;
    m_mode = mode;
    m_window.create(m_mode, m_name, sf::State::Fullscreen);
    m_window.setFramerateLimit(FRAMERATE_LIMIT);

    sf::View view(sf::FloatRect({0, 0}, {WINDOW_WIDTH, WINDOW_HEIGH}));

    float scaleX = static_cast<float>(mode.size.x) / WINDOW_WIDTH;
    float scaleY = static_cast<float>(mode.size.y) / WINDOW_HEIGH;
    float scale = std::min(scaleX, scaleY);

    float viewportWidth = WINDOW_WIDTH * scale / mode.size.x;
    float viewportHeight = WINDOW_HEIGH * scale / mode.size.y;
    float viewportLeft = (1.f - viewportWidth) / 2.f;
    float viewportTop = (1.f - viewportHeight) / 2.f;

    view.setViewport(sf::FloatRect({viewportLeft, viewportTop}, {viewportWidth, viewportHeight}));

    m_window.setView(view);
};

MyWindow::~MyWindow()
{

};

void MyWindow::Init()
{
    while (m_window.isOpen()) {
        Clear();
        CheckEvents();
        Update();
        Draw();
    }
};

void MyWindow::CheckEvents()
{
    while (const std::optional event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                m_window.close();
        } else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
            if (keyReleased->scancode == sf::Keyboard::Scancode::D)
                std::cout << "Test" << std::endl;
        }
    }
};

void MyWindow::Update()
{
    
};

void MyWindow::Clear()
{
    m_window.clear(sf::Color::Black);
};

void MyWindow::Draw()
{
    m_window.display();
};
