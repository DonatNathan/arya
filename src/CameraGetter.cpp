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

    // Convert BGR → RGBA
    cv::cvtColor(i_frame, i_frame, cv::COLOR_BGR2RGBA);

    if (i_texture.getSize().x != i_frame.cols ||
        i_texture.getSize().y != i_frame.rows)
    {
        i_image.resize({(unsigned)i_frame.cols, (unsigned)i_frame.rows});
        if (!i_texture.loadFromImage(i_image))
            std::cerr << getColorFromCode(Color::RED) << "Failed to load texture from image." << getColorFromCode(Color::RESET) << std::endl;
        i_sprite.setTexture(i_texture, true);
    }

    // std::memcpy(
    //     i_image.getPixelsPtr(),
    //     i_frame.ptr(),
    //     i_frame.total() * 4
    // );

    if (!i_texture.loadFromImage(i_image))
        std::cerr << getColorFromCode(Color::RED) << "Failed to load texture from image." << getColorFromCode(Color::RESET) << std::endl;
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
