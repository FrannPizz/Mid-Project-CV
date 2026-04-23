#include "../include/PreProc.h"

void blackWhiteFilterVector(std::vector<cv::Mat>& imageVector){
    for(int i = 0; i < imageVector.size(); i++){
        cv::cvtColor(imageVector[i], imageVector[i], cv::COLOR_BGR2GRAY);
    }
}

void gaussianFilterVector(std::vector<cv::Mat>& imageVector)
{
    for(int i = 0; i < imageVector.size(); i++){
        cv::GaussianBlur(imageVector[i], imageVector[i], cv::Size(5, 5), 1.5, 1.5);
    }
}

void preProcessFilter(std::vector<cv::Mat>& imageVector)
{
    blackWhiteFilterVector(imageVector);
    gaussianFilterVector(imageVector);
}