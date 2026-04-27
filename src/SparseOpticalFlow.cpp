#include "../include/SparseOpticalFlow.h"

//implements sparse optical flow to track features across frames and draw bounding box
cv::Rect sparseOpticalFlow(const std::vector<cv::Mat>& frames, const cv::Mat& mask) {
    //detetects good features in frame 0 using shi tomasi
    std::vector<cv::Point2f> features;
    cv::goodFeaturesToTrack(frames[0], features, 300, 0.05, 1.0, mask);

    //save initial features for later comparison
    std::vector<cv::Point2f> initialFeatures = features; 

    //using lucas kanade method, track the detected features across all frames, updating their positions
    for (size_t i = 0; i < frames.size() - 1; ++i) {

        std::vector<cv::Point2f> nextFeatures;
        std::vector<uchar> status;
        std::vector<float> err;

        cv::calcOpticalFlowPyrLK(frames[i], frames[i + 1], features, nextFeatures, status, err);

        for (size_t j = 0; j < nextFeatures.size(); ++j) {
            if (status[j] == 1)
                features[j] = nextFeatures[j];
        }
    }

    //calculate total movememt for every feature  between frame 0 e final frame
    std::vector<float> totalMovement;
    for (size_t j = 0; j < features.size(); ++j) {
        float dx = features[j].x - initialFeatures[j].x;
        float dy = features[j].y - initialFeatures[j].y;
        totalMovement.push_back(std::sqrt(dx * dx + dy * dy));
    }

    //sort total movement and select the 10% with the lowest as a threshold
    std::vector<float> sorted = totalMovement;
    std::sort(sorted.begin(), sorted.end());
    float thresh = sorted[int(sorted.size() * 0.10)];

    //keep the best 90% features and put into best features vector
    std::vector<cv::Point2f> bestFeatures;
    for (size_t j = 0; j < totalMovement.size(); ++j)
        if (totalMovement[j] >= thresh)
            bestFeatures.push_back(initialFeatures[j]);

    cv::Rect boundingBox = cv::boundingRect(bestFeatures);
    return boundingBox;
}
