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

    faceNet = cv::dnn::readNetFromCaffe(
        "../external/opencv/deploy.prototxt",
        "../external/opencv/res10_300x300_ssd_iter_140000.caffemodel"
    );

    if (faceNet.empty()) {
        std::cerr << "Failed to load DNN face model\n";
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

    cv::Mat blob = cv::dnn::blobFromImage(
        i_bgr_frame,
        1.0,
        cv::Size(300, 300),
        cv::Scalar(104, 177, 123),
        false,
        false
    );

    faceNet.setInput(blob);
    cv::Mat detections = faceNet.forward();

    float confidenceThreshold = 0.5;

    cv::Mat detectionMat(detections.size[2], detections.size[3], CV_32F, detections.ptr<float>());

    for (int i = 0; i < detectionMat.rows; i++) {
        float confidence = detectionMat.at<float>(i, 2);

        if (confidence > 0.5f) {
            int x1 = detectionMat.at<float>(i, 3) * i_bgr_frame.cols;
            int y1 = detectionMat.at<float>(i, 4) * i_bgr_frame.rows;
            int x2 = detectionMat.at<float>(i, 5) * i_bgr_frame.cols;
            int y2 = detectionMat.at<float>(i, 6) * i_bgr_frame.rows;

            cv::rectangle(i_bgr_frame, cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2)),
                        cv::Scalar(0, 255, 0), 2);
        }
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
