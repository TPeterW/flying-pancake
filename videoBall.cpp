#include <stdio.h>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include "physics.h"

using namespace cv;
using namespace std;

const char *win = "Flying Pancake";
static bool reverseMirror = false;

int main(int argc, char **argv)
{
    if (argc > 2) {
        fprintf(stderr, "Error: Please limit arguments to 1 or none.\n");
        exit(1);
    }
    else if (argc == 2) {
        if (!strcmp(argv[1], "-m"))
            reverseMirror = true;
        else {
            fprintf(stderr, "Usage: videoBall [-m]\n");
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

    Mat fgMaskMOG;          // foreground mask generated by MOG method
    Mat inputFrame, outFrame;
    Mat circ;
    Point pt;               // center of the ball

    BackgroundSubtractorMOG2 MOG;

    // initial position
    pt.x = 500;
    pt.y = 0;

    Point momentum;
    momentum.x = 10;
    momentum.y = 50;

    cap >> inputFrame;
    cvtColor(inputFrame, outFrame, CV_LOAD_IMAGE_COLOR);
    cvtColor(inputFrame, circ, CV_BGR2GRAY);

    int height = inputFrame.rows - 2 * RADIUS;
    int width = inputFrame.cols - 2 * RADIUS;
    
    int count = 0;
    int sum;
    Point small;
    Mat ballFrame, handFrame;
    Mat foregroundMask, backgroundMask;
    Mat reverseFrame;
    
    while (++count) {
        cap >> inputFrame;
        
        if (reverseMirror) {
            inputFrame.copyTo(reverseFrame);
            flip(reverseFrame, inputFrame, 1);      // 1 ~ flip against y axis
        }
        
        calcDir(&momentum, &pt, height, width);     // according to last frame

        MOG(inputFrame, fgMaskMOG);

        // blank canvas
        circ.setTo(Scalar(0,0,0));

        Rect ballRegion(pt.x - RADIUS, pt.y - RADIUS, 2 * RADIUS, 2 * RADIUS);
        ballFrame = circ(ballRegion);

        foregroundMask = fgMaskMOG > THRESH;         // have to put here otherwise floating point exception
        backgroundMask = fgMaskMOG <= THRESH;
        // fgMaskMOG.setTo(Scalar(255, 255, 255), foregroundMask);     // clean up
        fgMaskMOG.setTo(Scalar(0, 0, 0), backgroundMask);
        handFrame = fgMaskMOG(ballRegion);                          // cut to small size

        int halfRad = RADIUS / 2;
        // top left
        small.x = halfRad;    small.y = halfRad;
        sum = getOverlap(&ballFrame, &handFrame, &small);
        momentum.x += sum;              momentum.y += sum;

        // top right
        small.x = 3 * halfRad;   small.y = halfRad;
        sum = getOverlap(&ballFrame, &handFrame, &small);
        momentum.x -= sum;              momentum.y += sum;


        // bottom left
        small.x = halfRad;    small.y = 3 * halfRad;
        sum = getOverlap(&ballFrame, &handFrame, &small);
        momentum.x += sum;              momentum.y -= sum/3;

        // bottom right
        small.x = 3 * halfRad;   small.y = 3 * halfRad;
        sum = getOverlap(&ballFrame, &handFrame, &small);
        momentum.x -= sum;              momentum.y -= sum/3;
        
        // EVERYTHING BELOW THIS LINE SHOULD BE DRAWING THE outFrame
        outFrame.setTo(Scalar(0,0,0));      // set all of outFrame to be black
        outFrame.setTo(Scalar(255, 255, 255), foregroundMask);
        
        drawCircle(outFrame, pt, RADIUS, Scalar(255, 255, 255));

        imshow(win, outFrame);
        
        if (waitKey(1) >= 0)        // listening for key press
	        break;
    }

    return 0;
}
