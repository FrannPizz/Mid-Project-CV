#include "../include/PreProc.h"

//function for black and white filter
void blackWhiteFilterVector(std::vector<cv::Mat>& imageVector){
    for(int i = 0; i < imageVector.size(); i++){
        cv::cvtColor(imageVector[i], imageVector[i], cv::COLOR_BGR2GRAY);
    }
}
//function for gaussian filter
void gaussianFilterVector(std::vector<cv::Mat>& imageVector)
{
    for(int i = 0; i < imageVector.size(); i++){
        cv::GaussianBlur(imageVector[i], imageVector[i], cv::Size(5, 5), 1.5, 1.5);
    }
}
//function for pre processing the image vector that calls the black and white filter and the gaussian filter
void preProcessFilter(std::vector<cv::Mat>& imageVector)
{
    blackWhiteFilterVector(imageVector);
    //gaussianFilterVector(imageVector);
}