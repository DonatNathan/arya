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
        int i_cornerThickness = 3;
        int i_dotThickness = 2;
        int i_dotLen = 10;
        int i_crossLen = 5;
        int i_gapLen = 10;

    public:

        FaceCornerBracket(bool isMe, cv::Mat& frame, cv::Rect& rect);
        ~FaceCornerBracket();

        void setLabel();
        void setColor();

        void drawCornerRect();
        void drawDottedEdges();
        void draw(int tick);
        void cvDisplay(int tick);
};
