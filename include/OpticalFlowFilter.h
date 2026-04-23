#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/video/tracking.hpp>
#include <iostream>

#ifndef OPTICALFLOW_H_INCLUDED
#define OPTICALFLOW_H_INCLUDED

cv::Rect denseOpticalFlow(std::vector<cv::Mat>& imageVector); 

#endif
