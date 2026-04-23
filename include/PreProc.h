#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#ifndef PREPROC_H_INCLUDED
#define PREPROC_H_INCLUDED

void blackWhiteFilterVector(std::vector<cv::Mat>& imageVector); 
void gaussianFilterVector(std::vector<cv::Mat>& imageVector); 
void preProcessFilter(std::vector<cv::Mat>& imageVector);

#endif
