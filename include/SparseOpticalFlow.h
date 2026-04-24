#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/video/tracking.hpp>
#include <iostream>

#ifndef SPARSEOPTICALFLOW_H_INCLUDED
#define SPARSEOPTICALFLOW_H_INCLUDED

std::vector<cv::Point2f> detectFeatures(std::vector<cv::Mat>& imageVector);
cv::Rect sparseOpticalFlow(std::vector<cv::Mat>& imageVector); 

#endif