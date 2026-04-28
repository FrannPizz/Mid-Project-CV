#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>

#ifndef FRAMESTATS_H_INCLUDED
#define FRAMESTATS_H_INCLUDED

cv::Rect readGroundTruthBox(std::string& filePath);
float detectmIoU(cv::Rect& boundingBox, cv::Rect& groundTruthBox);
void detectAccuracy(std::vector<float>& mIoU);
void saveBoundingBox(const cv::Rect& boundingBox, const std::string& outputPath);


#endif