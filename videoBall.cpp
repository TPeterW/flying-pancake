/*
 * Joey Button
 *
 *
 */

#include <stdio.h>
#include <opencv2/video/background_segm.hpp>
#include "opencv2/opencv.hpp"

// #include <iostream>
// #include <sstream>



using namespace cv;
using namespace std;
#define RADIUS 32
#define THRESH 240.0f



Mat fgMaskMOG; //fg mask generated by MOG method

BackgroundSubtractorMOG2 MOG;

const char *win = "video";


void drawCircle(Mat img, Point center)
{
    int thickness = -1;
    int lineType = 8;

    circle( img,
            center,
            RADIUS,
            Scalar( 255, 255, 255 ),
            thickness,
            lineType);
}

void calcDir(Point *momentum, Point *pt, int height){
      pt->x += momentum->x;
      pt->y += momentum->y;


      if (pt->y < height) {
           // Accelerate due to Gravity
          momentum->y += 1;
      } else {
          momentum->x = momentum->x * 0.9;
          momentum->y = -(momentum->y * .5);   // bounce back up and halt it
          pt->y = height;
      }

      //off the top
      if (pt->y < RADIUS) {
            pt->y=RADIUS;
            momentum->y += 1;
            momentum->y = -(momentum->y * .5);
            momentum->x = momentum->x *0.9;
      }

      if (momentum->y * momentum->y <= 49 && pt->y > height){
          momentum->y = 0;
      }

}

int main()
{
    int cam = 0; // default camera
    VideoCapture cap(cam);
    if (!cap.isOpened()) {
        fprintf(stderr, "cannot open camera %d\n", cam);
        exit(1);
    }

    // namedWindow(win);
    namedWindow(win, 1);
    Mat inputFrame, outFrame;
    Point pt;
    pt.x = 500;
    pt.y = 0;

    Point momentum;
    momentum.x = 0;
    momentum.y = 100;

    int count = 0;

    cap >> inputFrame;
    cvtColor(inputFrame, outFrame, CV_LOAD_IMAGE_COLOR);

    int height = inputFrame.rows - 2 * RADIUS;
    // int width = inputFrame.cols - 2 * RADIUS;
    
    Mat mask;
    
    while (++count) {
        cap >> inputFrame;

        calcDir(&momentum, &pt, height);
        
        MOG(inputFrame, fgMaskMOG);


        // EVERYTHING ABOVE THIS SHOULD BE CALCULATING WHERE TO DRAW

        // EVERYTHING BELOW THIS LINE SHOULD BE DRAWING THE outFrame
        outFrame.setTo(Scalar(0,0,0));      // set all of outFrame to be black
        mask = fgMaskMOG > THRESH;            // have to put here otherwise floating point exception 
        outFrame.setTo(Scalar(255, 255, 255), mask);
        
        
        drawCircle(outFrame, pt);           // The real one
        imshow(win, outFrame);              // The real one

        if (waitKey(1) >= 0) // wait up to 30 msec
	        break;
    }

    return 0;
}

/*
    Mat BGRChannels[3];
    split(outFrame,BGRChannels); // split the BGR channesl
    BGRChannels[1]=Mat::zeros(outFrame.rows,outFrame.cols,CV_8UC1);// removing Green channel
    merge(BGRChannels,3,outFrame); // pack the image

    waitKey(0);
*/
