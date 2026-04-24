#include "../include/SparseOpticalFlow.h"
#include <algorithm>
#include <cmath>

std::vector<cv::Point2f> detectFeatures(std::vector<cv::Mat>& imageVector) {
    std::vector<cv::Point2f> features;
    cv::goodFeaturesToTrack(imageVector[0], features, 1000, 0.01, 10.0);
    return features;
}

cv::Rect sparseOpticalFlow(std::vector<cv::Mat>& imageVector) {

    std::vector<cv::Point2f> features = detectFeatures(imageVector);
    std::vector<cv::Point2f> initialFeatures = features;
    std::vector<cv::Point2f> nextFeatures;
    std::vector<uchar> status;
    std::vector<float> err;

    if (features.empty()) {
        std::cout << "Nessuna feature trovata nel frame 0" << std::endl;
        return cv::Rect();
    }

    int N = features.size();
    std::vector<float> totalmagnitude(N, 0.0f);

    for (int i = 0; i <imageVector.size(); i++) {

        cv::calcOpticalFlowPyrLK(imageVector[i], imageVector[i + 1], features, nextFeatures, status, err);

        for (int j = 0; j < N; j++) {
            if (status[j] == 1) {
                float dx = nextFeatures[j].x - features[j].x;
                float dy = nextFeatures[j].y - features[j].y;
                totalmagnitude[j] += std::sqrt(dx * dx + dy * dy);
                features[j] = nextFeatures[j]; // aggiorna posizione solo se trovata
            }
            // se status[j] == 0: features[j] rimane invariata, magnitude non aggiornata
        }
        int lost = 0;
        for (int j = 0; j < N; j++) if (!status[j]) lost++;
        std::cout << "Frame " << i << " | perse: " << lost << " | tracciate: " << (N - lost) << std::endl;
    }

    // Top 5%
    std::vector<float> sortedMags = totalmagnitude;
    std::sort(sortedMags.begin(), sortedMags.end());
    float threshold = sortedMags[static_cast<int>(N * 0.95)];
    std::cout << "top 5% threshold: " << threshold << std::endl;

    std::vector<cv::Point2f> movingFeatures;
    for (int k = 0; k < N; k++) {
        if (totalmagnitude[k] > threshold) {
            movingFeatures.push_back(initialFeatures[k]);

        }
    }
    std::cout << "Totale feature selezionate: " << movingFeatures.size() << std::endl;

    if (movingFeatures.empty()) return cv::Rect();
    return cv::boundingRect(movingFeatures);
}
