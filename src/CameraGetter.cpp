#include "CameraGetter.hpp"

CameraGetter::CameraGetter() : i_sprite(i_texture)
{
    faceNet = cv::dnn::readNetFromCaffe(
        "../external/opencv/deploy.prototxt",
        "../external/opencv/res10_300x300_ssd_iter_140000.caffemodel"
    );
    faceNet.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    faceNet.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    if (faceNet.empty()) {
        std::cerr << "Failed to load DNN face model\n";
    }

    embedder = cv::dnn::readNetFromTorch("../external/opencv/nn4.small2.v1.t7");
    embedder.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    embedder.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    if (embedder.empty()) {
        std::cerr << "Failed to load embedding model\n";
    }

    loadMyFaceDataset("../external/opencv/me");
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

    i_cap >> i_bgr_frame;
    if (i_bgr_frame.empty())
        return;

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

    cv::Mat detectionMat(
        detections.size[2],
        detections.size[3],
        CV_32F,
        detections.ptr<float>()
    );

    for (int i = 0; i < detectionMat.rows; i++) {
        float confidence = detectionMat.at<float>(i, 2);

        if (confidence < 0.5f)
            continue;

        int x1 = detectionMat.at<float>(i, 3) * i_bgr_frame.cols;
        int y1 = detectionMat.at<float>(i, 4) * i_bgr_frame.rows;
        int x2 = detectionMat.at<float>(i, 5) * i_bgr_frame.cols;
        int y2 = detectionMat.at<float>(i, 6) * i_bgr_frame.rows;

        cv::Rect faceRect(
            cv::Point(x1, y1),
            cv::Point(x2, y2)
        );

        faceRect &= cv::Rect(0, 0, i_bgr_frame.cols, i_bgr_frame.rows);

        if (faceRect.width < 20 || faceRect.height < 20)
            continue;

        cv::Mat face = i_bgr_frame(faceRect).clone();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            std::time_t now = std::time(nullptr);
            std::string timeString = std::ctime(&now);
            std::string path = "../external/opencv/me/cap_"
                            + timeString + ".jpg";
            cv::imwrite(path, face);
            std::cout << "Saved face sample: " << path << "\n";
        }

        cv::Mat faceBlob = cv::dnn::blobFromImage(
            face,
            1.0 / 255.0,
            cv::Size(96, 96),
            cv::Scalar(0, 0, 0),
            true,
            false
        );

        embedder.setInput(faceBlob);
        cv::Mat embedding = embedder.forward();

        cv::normalize(embedding, embedding);

        std::vector<double> dists;
        for (const auto& ref : myEmbeddings)
            dists.push_back(1.0 - embedding.dot(ref));

        std::sort(dists.begin(), dists.end());

        int K = std::min(3, (int)dists.size());
        double avgDist = std::accumulate(dists.begin(), dists.begin() + K, 0.0) / K;

        bool isMe = (avgDist < 0.3);

        // std::cout << "Dist(" << i << "): " << avgDist << std::endl;

        cv::Scalar color = isMe ?
            cv::Scalar(0, 255, 0) :
            cv::Scalar(0, 0, 255);

        cv::rectangle(i_bgr_frame, faceRect, color, 2);

        cv::putText(
            i_bgr_frame,
            isMe ? "YOU" : "OTHER",
            cv::Point(x1, y1 - 10),
            cv::FONT_HERSHEY_SIMPLEX,
            0.6,
            color,
            2
        );
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

void CameraGetter::loadMyFaceDataset(const std::string& folderPath)
{
    for (const auto& file : std::filesystem::directory_iterator(folderPath))
    {
        cv::Mat img = cv::imread(file.path().string());
        if (img.empty()) continue;

        cv::Mat blob = cv::dnn::blobFromImage(img, 1.0, cv::Size(300, 300),
                                               cv::Scalar(104, 177, 123), false, false);
        faceNet.setInput(blob);
        cv::Mat detections = faceNet.forward();
        cv::Mat detMat(detections.size[2], detections.size[3],
                       CV_32F, detections.ptr<float>());

        for (int i = 0; i < detMat.rows; i++) {
            if (detMat.at<float>(i, 2) < 0.5f) continue;

            int x1 = detMat.at<float>(i, 3) * img.cols;
            int y1 = detMat.at<float>(i, 4) * img.rows;
            int x2 = detMat.at<float>(i, 5) * img.cols;
            int y2 = detMat.at<float>(i, 6) * img.rows;

            cv::Rect faceRect(cv::Point(x1, y1), cv::Point(x2, y2));
            faceRect &= cv::Rect(0, 0, img.cols, img.rows);
            if (faceRect.width < 20 || faceRect.height < 20) continue;

            cv::Mat face = img(faceRect).clone();
            cv::Mat faceBlob = cv::dnn::blobFromImage(face, 1.0/255.0,
                                cv::Size(96, 96), cv::Scalar(0,0,0), true, false);

            embedder.setInput(faceBlob);
            cv::Mat emb = embedder.forward().clone();
            cv::normalize(emb, emb);
            myEmbeddings.push_back(emb);
            break;
        }
    }
}
