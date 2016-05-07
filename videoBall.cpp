/*
 * Joey Button
 *
 *
 */

#include <stdio.h>
#include "opencv/opencv.hpp"


Ptr<BackgroundSubtractor> pMOG2;
using namespace cv;
using namespace std;

const char *win = "video";


void drawCircle(Mat img, Point center)
{
    int thickness = -1;
    int lineType = 8;

    circle( img,
            center,
            32.0,
            Scalar( 255, 255, 255 ),
            thickness,
            lineType);
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
    Mat lastFrame, currentFrame, inputFrame, screen;
    Mat diffImage;  // diff
    Point pt;
    pt.x = 500;
    pt.y = 0;
    // int momentumY = 10;     // initial momentum hardcoded as 10
    // int momentumX = 0;

    cap >> lastFrame;
    cvtColor(lastFrame, lastFrame, CV_BGR2GRAY);

    float threshold = 5.0f;
    float dist;
    unsigned char pixel;
    int x, y;
    pMOG2 = createBackgroundSubtractorMOG2();
    double count = 0;
    while (1) {
        cap >> inputFrame;
        currentFrame.release();
        inputFrame.copyTo(currentFrame);

        // cvtColor(inputFrame, screen, CV_LOAD_IMAGE_COLOR);       // load from image

        // pt.x += momentumX;
        // pt.y += momentumY;

        // if (pt.y < 990) {                   // not at bottom
        //     momentumY += 1;
        // } else {
        //     momentumY = -(momentumY * .5);   // bounce back up and halt it
        // }
        // if (momentumY * momentumY <= 49 && pt.y > 990){
        //     momentumY = 0;
        // }


        // // This will zero out the entire image, and draw a red circle
        // Mat BGRChannels[3];
        // split(screen, BGRChannels);                                  // split the BGR channesl
        // BGRChannels[1] = Mat::zeros(screen.rows, screen.cols, CV_8UC1); // removing Green channel
        // BGRChannels[0] = Mat::zeros(screen.rows, screen.cols, CV_8UC1); // removing Blue channel
        // BGRChannels[2] = Mat::zeros(screen.rows, screen.cols, CV_8UC1); // removing Red channel

        // merge(BGRChannels, 3, screen);

        // drawCircle(screen, pt);

        // Find difference between two frames
        cvtColor(inputFrame, currentFrame, CV_BGR2GRAY);


        //diffImage.release();
        //absdiff(lastFrame, currentFrame, diffImage);
        pMOG2->apply(count++, currentFrame);
        // Mat foregroundMask = Mat::zeros(diffImage.rows, diffImage.cols, CV_8UC1);
        // float threshold = 30.0f;
        // float dist;
        // int x, y;
        // for (y = 0; y < diffImage.rows; ++y) {
        //     for (x = 0; x < diffImage.cols; ++x) {
        //         // Vec3b pixel = diffImage.at<Vec3b>(x, y);

        //         // dist = sqrt(pixel[0] * pixel[0] + pixel[1] * pixel[1] + pixel[2] * pixel[2]);

        //         // if (dist > threshold)       // big enough difference
        //         //     foregroundMask.at<Vec3b>(x, y) = 255;
        //         // printf("%d %d\n");
        //     }
        // }

        // GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        // Canny(edges, edges, 15, 30, 3);

        printf("%d\n", diffImage.cols);


        // char *haha;
        // std::cin >> haha;

        lastFrame.release();
        currentFrame.copyTo(lastFrame);

        imshow(win, diffImage);

        if (waitKey(10) >= 0) // wait up to 30 msec
	        break;
    }

    return 0;
}

/*
    Mat BGRChannels[3];
    split(screen,BGRChannels); // split the BGR channesl
    BGRChannels[1]=Mat::zeros(screen.rows,screen.cols,CV_8UC1);// removing Green channel
    merge(BGRChannels,3,screen); // pack the image

    waitKey(0);
*/
