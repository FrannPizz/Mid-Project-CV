#include "../include/PreProc.h"
#include "../include/SparseOpticalFlow.h"

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, char** argv)
{

    std::cout << "Avvio"<< std::endl;

    //check argv
    if (argc < 2) {
        std::cout << "Errore: Inserisci il percorso" << std::endl;
        cv::waitKey(0);
        return -1;
    }
    
    //save frame in vector as string
	std::vector<std::string> pathFrames;
	cv::glob(std::string(argv[1]) + "/*", pathFrames, false);
    std::sort(pathFrames.begin(), pathFrames.end());
	
    //convert frames string in cv::Mat
    std::vector<cv::Mat> frames;
    for (int i = 0; i < pathFrames.size(); i++){
        frames.push_back(cv::imread(pathFrames[i]));
    }

    //save the first frame colored for the bounding box 
    cv::Mat firstFrame = frames[0];

    //black&white and blur (gaussian filter) every frame
	preProcessFilter(frames);
	std::cout << "Pre Processed Frame" << std::endl;
    //return the rect with the bounding box of the area with the most motion
    cv::Rect boundingBox = sparseOpticalFlow(frames);

    //draw bounding box on the first frame
    cv::rectangle(firstFrame, boundingBox, cv::Scalar(0, 0, 255), 2);   

    //show the first frame with the bounding box
    cv::imshow("Bounding Box", firstFrame);
    cv::waitKey(0);
    
    return 0;
}
