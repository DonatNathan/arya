#include "modules/eyes/FaceCornerBracket.hpp"

FaceCornerBracket::FaceCornerBracket(bool isMe, cv::Mat& frame, cv::Rect& rect) : i_frame(frame), i_rect(rect)
{
    i_isMe = isMe;

    setLabel();
    setColor();
};

FaceCornerBracket::~FaceCornerBracket()
{

};

void FaceCornerBracket::setLabel()
{
    if (i_isMe)
        i_label = "ADMIN";
    else
        i_label = "UNKNOWN";
}

void FaceCornerBracket::setColor()
{
    if (i_isMe)
        i_color = cv::Scalar(EYES_YELLOW.b, EYES_YELLOW.g, EYES_YELLOW.r);
    else
        i_color = cv::Scalar(EYES_WHITE.b, EYES_WHITE.g, EYES_WHITE.r);
}

void FaceCornerBracket::drawCornerRect()
{
    int x = i_rect.x, y = i_rect.y;
    int w = i_rect.width, h = i_rect.height;

    // Top-left
    cv::line(i_frame, {x, y},           {x + i_cornerLen, y},           i_color, i_thickness);
    cv::line(i_frame, {x, y},           {x, y + i_cornerLen},           i_color, i_thickness);

    // Top-right
    cv::line(i_frame, {x + w, y},       {x + w - i_cornerLen, y},       i_color, i_thickness);
    cv::line(i_frame, {x + w, y},       {x + w, y + i_cornerLen},       i_color, i_thickness);

    // Bottom-left
    cv::line(i_frame, {x, y + h},       {x + i_cornerLen, y + h},       i_color, i_thickness);
    cv::line(i_frame, {x, y + h},       {x, y + h - i_cornerLen},       i_color, i_thickness);

    // Bottom-right
    cv::line(i_frame, {x + w, y + h},   {x + w - i_cornerLen, y + h},   i_color, i_thickness);
    cv::line(i_frame, {x + w, y + h},   {x + w, y + h - i_cornerLen},   i_color, i_thickness);
};

void FaceCornerBracket::drawScanEffect(int tick)
{
    int period = i_rect.height * 2;
    int t = tick % period;
    int scanY = (t < i_rect.height) ? i_rect.y + t : i_rect.y + period - t;

    cv::line(i_frame,
        {i_rect.x, scanY},
        {i_rect.x + i_rect.width, scanY},
        i_color * 0.5,
        1
    );
};

void FaceCornerBracket::drawOverlay()
{
    cv::Mat overlay = i_frame.clone();
    cv::rectangle(overlay, i_rect, i_color, cv::FILLED);
    cv::addWeighted(overlay, 0.08, i_frame, 0.92, 0, i_frame);
};

void FaceCornerBracket::draw(int tick)
{
    this->drawOverlay();
    this->drawCornerRect();
    this->drawScanEffect(tick);
};

void FaceCornerBracket::cvDisplay(int tick)
{
    this->draw(tick);

    int baseline = 0;
    cv::Size textSize = cv::getTextSize(i_label, cv::FONT_HERSHEY_SIMPLEX, 0.45, 1, &baseline);

    cv::Point textPos(
        i_rect.x + (i_rect.width - textSize.width) / 2,
        i_rect.y + i_rect.height + 15
    );

    cv::putText(i_frame, i_label, textPos,
                cv::FONT_HERSHEY_SIMPLEX, 0.45, i_color, 1);
};
