#include "../include/PreProc.h"
#include "../include/OpticalFlowFilter.h"
#include "../include/SparseOpticalFlow.h"
#include "../include/FrameStats.h"

int main() {

    std::cout << "Avvio" << std::endl;
    
    std::vector<std::string> pathFrames = {
        "../data/bird/",
        "../data/car/",
        "../data/frog/",
        "../data/sheep/",
        "../data/squirrel/"
    };

    std::vector<std::string> pathLabels = {
        "../labels/bird/",
        "../labels/car/",
        "../labels/frog/",
        "../labels/sheep/",
        "../labels/squirrel/"
    };

    std::vector<float> mIoUValues;

    for (int i = 0; i < pathFrames.size(); i++){
        
        std::vector<std::string> imageFiles;
        cv::glob(pathFrames[i] + "/*", imageFiles);

        std::vector<cv::Mat> frames;
        for (int i = 0; i < imageFiles.size(); i++) {
            frames.push_back(cv::imread(imageFiles[i]));
        }

        cv::Mat firstFrame = frames[0];


        //CALCULATEGREEN RECT PART
        std::string labelPath = pathLabels[i] + "0000.txt";

        cv::Rect groundTruthBox = readGroundTruthBox(labelPath);

        // CALCULATE RED RECT PART
        preProcessFilter(frames);

        cv::Mat maskFarneback = createMaskFarneback(frames);

        cv::Rect boundingBox = sparseOpticalFlow(frames, maskFarneback);

        //PRINT GREEN RECT
        cv::rectangle(firstFrame, groundTruthBox, cv::Scalar(0, 255, 0), 2);
        //PRINT RED RECT
        cv::rectangle(firstFrame, boundingBox, cv::Scalar(0, 0, 255), 2);


        mIoUValues.push_back(detectmIoU(boundingBox, groundTruthBox));

        cv::imshow("Bounding Box", firstFrame);
        cv::waitKey(5000);

        std::cout << "Done with " << pathFrames[i] << std::endl;
    }
    detectAccuracy(mIoUValues);
    return 0;
}
