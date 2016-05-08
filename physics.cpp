#include <stdio.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

Mat calcMovement(Mat ballFrame, Mat cameraFrame)
{
    Mat outFrame;
    bitwise_and(ballFrame, cameraFrame, outFrame);      // temporary
    
    return outFrame;
}