//Author: Francesco Pizzato
#include "../include/CreateMaskFarneback.h"

//creates a mask by computing dense optical flow between consecutive frames using farneback algorithm for help sparse optical flow
cv::Mat createMaskFarneback(std::vector<cv::Mat>& frames) {
    
    //initialize the future mask 
    cv::Mat nullMask = cv::Mat::zeros(frames[0].size(), CV_32F);

    //loop through consecutive frame pairs to compute optical flow
    for (int i = 0; i < frames.size() - 1; i++) {
        
        //calculate exponential weight for this frame pair (more recent frames have higher weight)
        float weight = std::exp(-i);

        //compute dense optical flow using farneback algorithm
        cv::Mat flow;
        cv::calcOpticalFlowFarneback(frames[i], frames[i + 1], flow, 0.5, 3, 15, 3, 5, 1.2, 0);

        //split flow into x and y components
        cv::Mat flowAxes[2];
        cv::split(flow, flowAxes);

        //convert cartesian coordinates to polar (magnitude and angle)
        cv::Mat magnitude;
        cv::Mat angle;
        cv::cartToPolar(flowAxes[0], flowAxes[1], magnitude, angle);

        //accumulate weighted magnitude into the mask
        nullMask += weight * magnitude;
    }

    //normalize nullMask to 0-255 range for thresholding and convert to 8-bit unsigned integer type
    cv::Mat accNorm;
    cv::normalize(nullMask, accNorm, 0, 255, cv::NORM_MINMAX, CV_8U);
    
    //compute otsu threshold then raise it by 30% to cut weak motion regions
    cv::Mat binaryMask;
    double otsuThresh = cv::threshold(accNorm, binaryMask, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    cv::threshold(accNorm, binaryMask, otsuThresh, 255, cv::THRESH_BINARY);

    //find connected components in the binary mask
    cv::Mat labels, stats, centroids;
    int numComp = cv::connectedComponentsWithStats(binaryMask, labels, stats, centroids);

    int bestLabel = -1;
    int bestArea = 0;

    //find the largest blob
    for (int i = 1; i < numComp; i++) {
        int area = stats.at<int>(i, cv::CC_STAT_AREA);
        if (area > bestArea) {
            bestArea = area;
            bestLabel = i;
        }
    }

    //expand the largest blob by 1px to detect which other blobs touch it
    cv::Mat largestBlob = (labels == bestLabel);
    cv::Mat expandedBlob;
    cv::Mat touchKernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::dilate(largestBlob, expandedBlob, touchKernel);

    //include all blobs that touch the largest one, discard isolated ones
    cv::Mat finalMask = cv::Mat::zeros(binaryMask.size(), CV_8U);
    for (int i = 1; i < numComp; i++) {
        cv::Mat blobMask = (labels == i);
        cv::Mat overlap;
        cv::bitwise_and(blobMask, expandedBlob, overlap);
        if (cv::countNonZero(overlap) > 0)
            finalMask |= blobMask;
    }

    return finalMask;
}
