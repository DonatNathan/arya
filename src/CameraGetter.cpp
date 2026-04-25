#include "CameraGetter.hpp"

CameraGetter::CameraGetter() : i_sprite(i_texture)
{

};

CameraGetter::~CameraGetter()
{
    stop();
};

bool CameraGetter::start(int device)
{
    if (i_running)
        return true;

    i_cap.open(device);
    if (!i_cap.isOpened()) {
        std::cerr << getColorFromCode(Color::RED) << "[Camera] Failed to open device" << getColorFromCode(Color::RESET) << std::endl;
        return false;
    }

    i_cap.set(cv::CAP_PROP_FPS, 30);

    i_running = true;
    return true;
};


void CameraGetter::stop()
{
    if (i_running) {
        i_cap.release();
        i_running = false;
    }
}

bool CameraGetter::isRunning() const {
    return i_running;
}

void CameraGetter::update()
{
    if (!i_running)
        return;

    i_cap >> i_frame;
    if (i_frame.empty())
        return;

    cv::cvtColor(i_frame, i_frame, cv::COLOR_BGR2RGBA);

    if (i_texture.getSize().x != i_frame.cols ||
        i_texture.getSize().y != i_frame.rows)
    {
        i_texture = sf::Texture({(unsigned int)i_frame.cols, (unsigned int)i_frame.rows});
        i_sprite.setTexture(i_texture, true);
    }

    i_texture.update(i_frame.ptr());
}

void CameraGetter::draw(sf::RenderWindow& window)
{
    if (!i_running)
        return;

    sf::Vector2f viewSize = window.getView().getSize();
    sf::Vector2u texSize = i_texture.getSize();

    i_sprite.setScale({viewSize.x / texSize.x, viewSize.y / texSize.y});

    i_sprite.setPosition(window.getView().getCenter() - viewSize / 2.f);

    window.draw(i_sprite);
}
