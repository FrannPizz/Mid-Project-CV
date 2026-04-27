#include <opencv2/imgproc.hpp>
#include <opencv2/video/tracking.hpp>

#ifndef MASKFARNEBACK_H_INCLUDED
#define MASKFARNEBACK_H_INCLUDED

cv::Mat createMaskFarneback(std::vector<cv::Mat>& frames);

#endif