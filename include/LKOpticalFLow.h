#include <opencv2/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <cmath>
#include <algorithm>

#ifndef LKOPTICALFLOW_H_INCLUDED
#define LKOPTICALFLOW_H_INCLUDED

cv::Rect LKOpticalFlow(const std::vector<cv::Mat>& frames, const cv::Mat& mask);

#endif