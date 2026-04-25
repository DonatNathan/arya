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

    if (!faceCascade.load("../external/opencv/haarcascade_frontalface_default.xml")) {
        std::cerr << "Failed to load face cascade\n";
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

    i_cap >> i_bgr_frame;
    if (i_bgr_frame.empty())
        return;

    cv::Mat gray;
    cv::cvtColor(i_bgr_frame, gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray, gray);

    std::vector<cv::Rect> faces;
    faceCascade.detectMultiScale(
        gray,
        faces,
        1.1,
        3,
        0,
        cv::Size(30, 30)
    );

    for (const auto& face : faces) {
        cv::rectangle(i_bgr_frame, face, cv::Scalar(0, 255, 0), 2);
    }

    cv::cvtColor(i_bgr_frame, i_rgba_frame, cv::COLOR_BGR2RGBA);

    if (i_texture.getSize().x != i_rgba_frame.cols ||
        i_texture.getSize().y != i_rgba_frame.rows)
    {
        i_texture = sf::Texture({(unsigned int)i_rgba_frame.cols, (unsigned int)i_rgba_frame.rows});
        i_sprite.setTexture(i_texture, true);
    }

    i_texture.update(i_rgba_frame.ptr());
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
