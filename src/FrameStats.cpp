#include "../include/FrameStats.h"


//reads ground truth box from text file and returns it as a cv::Rect object
cv::Rect readGroundTruthBox(std::string& filename) {
    //open the text file and read the coordinates of the box
    std::ifstream file(filename);
    int x1, y1, x2, y2;
    file >> x1 >> y1 >> x2 >> y2;
    //return the ground truth box 
    return cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2));
}

//calculates mean Intersection over Union (mIoU) between the predicted bounding box and the ground truth box
float detectmIoU(cv::Rect& boundingBox, cv::Rect& groundTruthBox){

    //calculate the area of intersection
    int xA = std::max(boundingBox.x, groundTruthBox.x);
    int yA = std::max(boundingBox.y, groundTruthBox.y);
    int xB = std::min(boundingBox.x + boundingBox.width, groundTruthBox.x + groundTruthBox.width);
    int yB = std::min(boundingBox.y + boundingBox.height, groundTruthBox.y + groundTruthBox.height);

    int interArea = std::max(0, xB - xA) * std::max(0, yB - yA);

    //calculate the union area
    int boxAArea = boundingBox.width * boundingBox.height;
    int boxBArea = groundTruthBox.width * groundTruthBox.height;
    int unionArea = boxAArea + boxBArea - interArea;

    //calculate mIoU
    float mIoU = float(interArea) / float(unionArea);

    return mIoU;
}

//calculates and prints the accuracy of the detection based on the mIoU values for all categories
void detectAccuracy(std::vector<float>& mIoU){

    int truePositive = 0;
    int falsePositive = 0; 

    //iterate through all mIoU values and count true positives and false positives
    for (int i = 0; i < mIoU.size(); i++) {
        if (mIoU[i] > 0.5) {
            truePositive++;
            std::cout << "Category " << i+1 << ": mIoU = " << mIoU[i] << " (True Positive)" << std::endl;
        } else {
            falsePositive++;
            std::cout << "Category " << i+1 << ": mIoU = " << mIoU[i] << " (False Positive)" << std::endl;
        }
    }
    std::cout << "Total objects categories: " << mIoU.size() << std::endl;
    std::cout << "True Positives: " << truePositive << std::endl;
    std::cout << "False Positives: " << falsePositive << std::endl;
    std::cout << "Accuracy: " << float(truePositive) / mIoU.size() * 100 << "%" << std::endl;

}