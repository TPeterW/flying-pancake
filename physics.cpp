#include <stdio.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

Mat calcMovement(cv::Mat ballFrame, cv::Mat cameraFrame)
{
    cv::Mat outFrame;
    bitwise_and(ballFrame, cameraFrame, outFrame);
    
    return outFrame;
}