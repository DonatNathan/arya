#include <SFML/Graphics.hpp>
#include <opencv2/opencv.hpp>

#include "Globals.hpp"
#include "Utils.hpp"

#pragma once

class CameraGetter {
    private:
        cv::VideoCapture i_cap;
        cv::Mat i_frame;

        sf::Image i_image;
        sf::Texture i_texture;
        sf::Sprite i_sprite;

        bool i_running = false;

    public:
        CameraGetter();
        ~CameraGetter();

        bool start(int device = 0);
        void stop();

        void update();
        void draw(sf::RenderWindow& window);

        bool isRunning() const;
};
