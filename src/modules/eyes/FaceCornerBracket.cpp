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
    cv::line(i_frame, {x, y},           {x + i_dotLen, y},           i_color, i_cornerThickness);
    cv::line(i_frame, {x, y},           {x, y + i_dotLen},           i_color, i_cornerThickness);

    // Top-right
    cv::line(i_frame, {x + w, y},       {x + w - i_dotLen, y},       i_color, i_cornerThickness);
    cv::line(i_frame, {x + w, y},       {x + w, y + i_dotLen},       i_color, i_cornerThickness);

    // Bottom-left
    cv::line(i_frame, {x, y + h},       {x + i_dotLen, y + h},       i_color, i_cornerThickness);
    cv::line(i_frame, {x, y + h},       {x, y + h - i_dotLen},       i_color, i_cornerThickness);

    // Bottom-right
    cv::line(i_frame, {x + w, y + h},   {x + w - i_dotLen, y + h},   i_color, i_cornerThickness);
    cv::line(i_frame, {x + w, y + h},   {x + w, y + h - i_dotLen},   i_color, i_cornerThickness);
};

void FaceCornerBracket::drawDottedEdges()
{
    int x = i_rect.x, y = i_rect.y;
    int w = i_rect.width, h = i_rect.height;
    int cx = x + w / 2, cy = y + h / 2;

    int skip = i_dotLen + i_gapLen;

    // Horizontal edges (top & bottom)
    for (int edge = 0; edge < 2; edge++) {
        int ey = (edge == 0) ? y : y + h;
        int dir = (edge == 0) ? 1 : -1;

        for (int dx = x + skip; dx < x + w - skip; dx += i_dotLen + i_gapLen) {
            int ex = std::min(dx + i_dotLen, x + w - skip);
            cv::line(i_frame, {dx, ey}, {ex, ey}, i_color, i_dotThickness);
        }

        cv::line(i_frame, {cx, ey}, {cx, ey + i_crossLen * 2 * dir}, i_color, i_dotThickness);
    }

    // Vertical edges (left & right)
    for (int edge = 0; edge < 2; edge++) {
        int ex = (edge == 0) ? x : x + w;
        int dir = (edge == 0) ? 1 : -1;

        for (int dy = y + skip; dy < y + h - skip; dy += i_dotLen + i_gapLen) {
            int ey = std::min(dy + i_dotLen, y + h - skip);
            cv::line(i_frame, {ex, dy}, {ex, ey}, i_color, i_dotThickness);
        }

        cv::line(i_frame, {ex, cy}, {ex + i_crossLen * 2 * dir, cy}, i_color, i_dotThickness);
    }
};

void FaceCornerBracket::draw(int tick)
{
    this->drawCornerRect();
    this->drawDottedEdges();
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
