/* showimg.cpp - display image in window
 *
 * CS 453 openCV demo
 */

#include <stdio.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

// global variable for window name
const char *win = "My image window";

int main(int argc, char **argv)
{
    if (argc < 2) {
	fprintf(stderr, "usage: %s img\n", argv[0]);
	exit(1);
    }

    Mat img = imread(argv[1], 1);
    if (img.empty()) { 
	fprintf(stderr, "cannot read image %s\n", argv[1]); 
	exit(1); 
    }
    //namedWindow(win, CV_WINDOW_AUTOSIZE); // window size will fit iamge size
    namedWindow(win, 0); // allows resizing (usually not a good idea)
    imshow(win, img);

    printf("hit a key to quit\n");
    waitKey(0);

    return 0;
}
