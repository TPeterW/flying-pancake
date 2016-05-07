/* Joey Button
 *
 *
 */

#include <stdio.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

const char *win = "video";


void drawCircle( Mat img, Point center )
{
 int thickness = -1;
 int lineType = 8;

 circle( img,
         center,
         32.0,
         Scalar( 0, 0, 255 ),
         thickness,
         lineType );
}

int main()
{
    int cam = 0; // default camera
    VideoCapture cap(cam);
    if (!cap.isOpened()) {
	fprintf(stderr, "cannot open camera %d\n", cam);
	exit(1);
    }

    //namedWindow(win);
    namedWindow(win, 1);
    Mat frame, screen;
    Point pt;
    pt.x = 500;
    pt.y = 0;
    int momentumY = 10;
    int momentumX = 0;

    while (1) {
        cap >> frame;
        cvtColor(frame, screen, CV_LOAD_IMAGE_COLOR);

        pt.x += momentumX;
        pt.y += momentumY;

        if(pt.y<990){
          momentumY += 1;
        }else {
          momentumY = -(momentumY/2);
        }
        if (momentumY*momentumY <= 49 && pt.y > 990){
          momentumY =0;
        }


        //This will zero out the entire image, and draw a red circle

        Mat BGRChannels[3];
        split(screen,BGRChannels);                                  // split the BGR channesl
        BGRChannels[1]=Mat::zeros(screen.rows,screen.cols,CV_8UC1); // removing Green channel
        BGRChannels[0]=Mat::zeros(screen.rows,screen.cols,CV_8UC1); // removing Green channel
        BGRChannels[2]=Mat::zeros(screen.rows,screen.cols,CV_8UC1); // removing Green channel
        merge(BGRChannels,3,screen);

        drawCircle(screen,pt);


                            // pack the image


        imshow(win, screen);
        if (waitKey(30) >= 0) // wait up to 30 msec
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
