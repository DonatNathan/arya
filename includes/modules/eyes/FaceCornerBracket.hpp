#include <opencv2/opencv.hpp>

#include "Globals.hpp"

#pragma once

class FaceCornerBracket {
    private:
        bool i_isMe;
        cv::Mat& i_frame;
        cv::Rect& i_rect;
        std::string i_label;
        cv::Scalar i_color;
        int i_thickness = 2;
        int i_cornerLen = 20;

    public:

        FaceCornerBracket(bool isMe, cv::Mat& frame, cv::Rect& rect);
        ~FaceCornerBracket();

        void setLabel();
        void setColor();

        void drawCornerRect();
        void drawScanEffect(int tick);
        void drawOverlay();
        void draw(int tick);
        void cvDisplay(int tick);
};
