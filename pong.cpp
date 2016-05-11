#include <stdio.h>
#include <opencv2/video/background_segm.hpp>
#include "opencv2/opencv.hpp"
#include "physics.h"

using namespace cv;
using namespace std;

const char *win = "Flying Pancake";
static bool reverseMirror = true;

int main(int argc, char **argv)
{
    if (argc > 2) {
        fprintf(stderr, "Error: Please limit arguments to 1 or none.\n");
        exit(1);
    }
    else if (argc == 2) {
        if (!strcmp(argv[1], "-r"))
            reverseMirror = false;
        else {
            fprintf(stderr, "Usage: videoBall [-r]\n");
            exit(1);
        }
    }
    
    int cam = 0; // default camera
    VideoCapture cap(cam);
    if (!cap.isOpened()) {
        fprintf(stderr, "Error: Cannot open camera %d\n", cam);
        exit(1);
    }
    
    namedWindow(win, CV_WINDOW_AUTOSIZE);
    
    Mat fgMaskMOG;
    BackgroundSubtractorMOG2 MOG;
    
    Mat inputFrame;
    cap >> inputFrame;
    
    int height = inputFrame.rows;
    int width = inputFrame.cols;
    
    // initial position
    Point pt;
    pt.x = width / 2;
    pt.y = height / 2;
    
    // initial momentum
    Point momentum;
    momentum.x = 20;
    momentum.y = -10;
    
    int count = 0;
    int sum;
    Mat reverseFrame
    Mat ballFrame, handFrame;
    Point small;
    while(++count) {
        cap >> inputFrame;
        
        if (reverseMirror) {
            inputFrame.copyTo(reverseFrame);
            flip(reverseFrame, inputFrame, 1);
        }
        
        MOG(inputFrame, fgMaskMOG);
        
        switch (pongDir(&momentum, &pt, height, width)) {
            case 0: // touches left wall
                break;
            default:
                drawCircle(fgMaskMOG, pt, RADIUS);
                imshow(win, fgMaskMOG);
                goto repeat;
        }
        
        // blank canvas
        circ.setTo(Scalar(0, 0, 0));
        Rect ballRegion(pt.x - RADIUS, pt.y - RADIUS, 2 * RADIUS, 2 * RADIUS);
        ballFrame = circ(ballRegion);
        
        foregroundMask = fgMaskMOG > THRESH;
        backgroundMask = fgMaskMOG <= THRESH;
        fgMaskMOG.setTo(Scalar(255, 255, 255), foregroundMask);
        fgMaskMOG.setTo(Scalar(0, 0, 0), backgroundMask);
        handFrame = fgMaskMOG(ballRegion);
        
        int halfRad = RADIUS / 2;
        
        // top left
        small.x = halfRad;   small.y = halfRad;
        sum = getOverlap(&ballFrame, &handFrame, &small);
        if (pt.x < width / 2)
            momentum.x += sum > 0 ? sum * 3 : 0;
        else
            momentum.x += sum < 0 ? sum * 3 : 0;
        momentum.y += sum * 3;
        
        // top right
        small.x = 3 * halfRad;  small.y = halfRad;
        sum = getOverlap(&ballFrame, &handFrame, &small);
        if (pt.x < width / 2)
            momentum.x += sum > 0 ? sum * 3 : 0;
        else
            momentum.x += sum < 0 ? sum * 3 : 0;
        
        // TODO:
        
        
        repeat:
        
        if (waitKey(1) >= 0)        // listening for key press
	        break;
    }
    
}