#include <SFML/Graphics.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <numeric>
#include <ctime>
#include <string>

#include "Globals.hpp"
#include "Utils.hpp"
#include "modules/eyes/FaceCornerBracket.hpp"

#pragma once

class CameraGetter {
    private:
        cv::VideoCapture i_cap;
        cv::Mat i_bgr_frame;
        cv::Mat i_rgba_frame;

        cv::dnn::Net faceNet;
        cv::dnn::Net embedder;

        std::vector<cv::Mat> myEmbeddings;

        sf::Image i_image;
        sf::Texture i_texture;
        sf::Sprite i_sprite;

        bool i_running = false;
        int i_tick = 0;

    public:
        CameraGetter();
        ~CameraGetter();

        bool start(int device = 0);
        void stop();

        void update();
        void draw(sf::RenderWindow& window);

        bool isRunning() const;
        void loadMyFaceDataset(const std::string& folderPath);
};
