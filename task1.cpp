#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

cv::Mat img;
cv::Mat img2;
int threshold1 = 80;
int threshold2 = 200;

void onTrackbar(int val, void* userData) {
    cv::Canny(img, img2, threshold1, threshold2);
    cv::imshow("Image Canny detection", img2);
}

int main(int argc, char** argv)
{
 img = cv::imread("street_scene.png");
 if (img.empty()) {
    std::cout << "Could not read the image: " << argv[1] << std::endl;
    return -1;
 }
cv::imshow("Image 1", img);

cv::namedWindow("Image Canny detection", cv::WINDOW_AUTOSIZE);
cv::createTrackbar("Threshold1", "Image Canny detection", &threshold1, 500, onTrackbar);
cv::createTrackbar("Threshold2", "Image Canny detection", &threshold2, 500, onTrackbar);

cv::Canny(img, img2, threshold1, threshold2);
cv::imshow("Image Canny detection", img2);

cv::waitKey(0);

return 0;
}