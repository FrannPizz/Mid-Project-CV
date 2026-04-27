#include "../include/PreProc.h"
#include "../include/OpticalFlowFilter.h"
#include "../include/SparseOpticalFlow.h"
#include "../include/FrameStats.h"

int main() {

    // Start processing and notify the user.
    std::cout << "Avvio" << std::endl;
    
    //paths to image folders
    std::vector<std::string> pathFrames = {
        "../data/bird/",
        "../data/car/",
        "../data/frog/",
        "../data/sheep/",
        "../data/squirrel/"
    };

    //paths to corresponding ground-truth label folders
    std::vector<std::string> pathLabels = {
        "../labels/bird/",
        "../labels/car/",
        "../labels/frog/",
        "../labels/sheep/",
        "../labels/squirrel/"
    };

    //collected IoU values for each processed sequence
    std::vector<float> mIoUValues;

    //iterate and perform detection over each dataset folder
    for (int i = 0; i < pathFrames.size(); i++){
        
        //collect all image file paths in the current folder
        std::vector<std::string> imageFiles;
        cv::glob(pathFrames[i] + "/*", imageFiles);

        //load frames into imageFiles vector
        std::vector<cv::Mat> frames;
        for (int j = 0; j < imageFiles.size(); j++) {
            frames.push_back(cv::imread(imageFiles[j]));
        }

        //save the first frame for visualization
        cv::Mat firstFrame = frames[0];

        //build the label path for the ground-truth bounding box
        std::string labelPath = pathLabels[i] + "0000.txt";
        cv::Rect groundTruthBox = readGroundTruthBox(labelPath);

        //preprocess frames before computing optical flow
        preProcessFilter(frames);

        //create a motion mask using Farneback optical flow
        cv::Mat maskFarneback = createMaskFarneback(frames);

        //compute the detected bounding box using sparse optical flow
        cv::Rect boundingBox = sparseOpticalFlow(frames, maskFarneback);

        //draw the ground-truth box in green and the detection box in red
        cv::rectangle(firstFrame, groundTruthBox, cv::Scalar(0, 255, 0), 2);
        cv::rectangle(firstFrame, boundingBox, cv::Scalar(0, 0, 255), 2);

        //compute and store the intersection-over-union metric (mIoU)
        mIoUValues.push_back(detectmIoU(boundingBox, groundTruthBox));

        //display the first frame with annotated boxes
        cv::imshow("Bounding Box", firstFrame);
        cv::waitKey(5000);

        std::cout << "Done with " << pathFrames[i] << std::endl;
    }

    //evaluate overall detection accuracy after all sequences
    detectAccuracy(mIoUValues);
    return 0;
}
