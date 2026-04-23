#include "../include/OpticalFlowFilter.h"

cv::Rect denseOpticalFlow(std::vector<cv::Mat>& imageVector){

    //total magnitude of the flow
    cv::Mat totalMagnitude = cv::Mat::zeros(imageVector[0].size(), CV_32F);

    for(int i = 0; i < imageVector.size() - 1; i++){
        
        //calcuate flow between two frames
        cv::Mat flow;
        cv::calcOpticalFlowFarneback(imageVector[0], imageVector[i + 1], flow, 0.5, 3, 15, 3, 5, 1.2, 0);

        //split the flow in x and y
        cv::Mat flowParts[2];
        cv::split(flow, flowParts);

        //calculate the magnitude for each pixel and convert to polar coordinates, angle is empty but cartToPolar needs a placeholder
        cv::Mat magnitude;
        cv::Mat angle;
        cv::cartToPolar(flowParts[0], flowParts[1], magnitude, angle);
        
        //accumulate the magnitude of the flow
        totalMagnitude += magnitude;
    
        /*
        -------------------------------------------------------------------
        PARTE VISIVA CHE SALTERA
        -------------------------------------------------------------------
        */

        //create an image to show the flow
        cv::Mat flowImage;
        cv::cvtColor(imageVector[i], flowImage, cv::COLOR_GRAY2BGR);

        for(int y = 0; y < flow.rows; y += 10){
            for(int x = 0; x < flow.cols; x += 10){
                const cv::Point2f& fxy = flow.at<cv::Point2f>(y, x);
                cv::line(flowImage, cv::Point(x, y), cv::Point(cvRound(x + fxy.x), cvRound(y + fxy.y)), cv::Scalar(0, 255, 0));
                cv::circle(flowImage, cv::Point(x, y), 1, cv::Scalar(0, 255, 0), -1);
            }
        }

        //show the flow
        cv::imshow("Dense Optical Flow", flowImage);
        cv::waitKey(30);

         /*
        -------------------------------------------------------------------
        PARTE VISIVA CHE SALTERA
        -------------------------------------------------------------------
        */

    }

    //threshold the total magnitude to create a binary mask of the areas with significant motion
    // normalizza in CV_8U per Otsu
    cv::Mat magnitudeNorm;
    cv::normalize(totalMagnitude, magnitudeNorm, 0, 255, cv::NORM_MINMAX, CV_8U);

    // Otsu threshold automatico
    cv::Mat mask;

    //OCCHIO CHE QUA IL THRESHOLD E ALZATO A MANO DOPO L'OTSU  PER STRINGERE DI PIU IL RISULTATO, SE SI VUOLE PROVARE CON SOLO OTSU BASTA COMMENTARE LA RIGA DOPO E DECOMMENTARE QUELLA DELL'OTSU   
    //cv::threshold(magnitudeNorm, mask, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    double otsuThresh = cv::threshold(magnitudeNorm, mask, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    // alza il threshold manualmente oltre Otsu
    cv::threshold(magnitudeNorm, mask, otsuThresh * 1.5, 255, cv::THRESH_BINARY);

    //scan the entire mask and collect the coordinates of all non-zero pixels
    std::vector<cv::Point> points;
    cv::findNonZero(mask, points);

    //check if there is movement, if not return an empty rectangle
    if(points.empty()){
        std::cout << "Nessun movimento rilevato" << std::endl;
        return cv::Rect(0, 0, 0, 0);
    }

    //compute the bounding box of the points
    cv::Rect rect = cv::boundingRect(points);

    return rect;
}
