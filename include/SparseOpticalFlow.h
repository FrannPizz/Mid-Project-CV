#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/video/tracking.hpp>
#include <iostream>
#include <cmath>
#include <algorithm>

#ifndef SPARSEOPTICALFLOW_H_INCLUDED
#define SPARSEOPTICALFLOW_H_INCLUDED

cv::Rect sparseOpticalFlow(const std::vector<cv::Mat>& frames, const cv::Mat& mask);

#endif
