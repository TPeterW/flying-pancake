#include <stdio.h>
#include <opencv2/video/background_segm.hpp>
#include "opencv2/opencv.hpp"
#include "physics.h"

using namespace cv;
using namespace std;

const char *win = "Flying Pancake";
static bool reverseMirror = true;

void game_over(int right, int left, Mat scoreFrame, VideoCapture cap ){
    Mat inFrame;
    cap >> inFrame;

    while (1){
      char c = waitKey(30);
      if (c >= 0) {
          if (c == 'r') {
            //Start a new game
              return 0;
          }
          else if ( c == 'd'){
            //should be done
            exit(0);

          }
          else
              continue;
      }

    }

}

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

    Mat inputFrame, outFrame, scoreFrame;
    cap >> inputFrame;

    int height = inputFrame.rows;
    int width = inputFrame.cols;

    // initial position
    Point pt;
    pt.x = RADIUS;
    pt.y = height / 4;

    // initial momentum
    Point momentum;
    momentum.x = 0;
    momentum.y = 0;

    Mat circ;
    cvtColor(inputFrame, outFrame, CV_LOAD_IMAGE_COLOR);
    cvtColor(inputFrame, scoreFrame, CV_LOAD_IMAGE_COLOR);
    cvtColor(inputFrame, circ, CV_BGR2GRAY);

    int count = 0;
    int sum;
    Mat reverseFrame;
    Mat ballFrame, handFrame;
    Mat foregroundMask, backgroundMask;

    Point small;
    int score = 0, left = 0, right = 0;
    int timer = 50; // 50 frames between points are scored


    /*
    A score frame is used because drawing text is a VERY expensive operation,
    adding two images together is almost 10x faster than using the putText function

    SO:
      A mask is created with the current score, and it is only updated between
      points being scored.
    */
    // initialize the scoreFrame here
    string text = "First to 5 Points Wins";

    int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
    int baseline=0;
    double fontScale = 3;
    Size textSize = getTextSize(text, fontFace,
                              fontScale, 8, &baseline);


    // center the text
    Point textOrg((scoreFrame.cols - textSize.width)/2,
                  (scoreFrame.rows + textSize.height)/2);
    scoreFrame = scoreFrame > 256;

    putText(scoreFrame, "First to 5 points wins", textOrg, fontFace, fontScale,
                Scalar(255,255,255), 1, 8, false);

    // Hackish way to Initialize the game
    right = -1;
    score = -1;

    while(++count) {
        cap >> inputFrame;

        if (reverseMirror) {
            inputFrame.copyTo(reverseFrame);
            flip(reverseFrame, inputFrame, 1);
        }
        cvtColor(inputFrame, outFrame, CV_LOAD_IMAGE_COLOR);
        MOG(inputFrame, fgMaskMOG);

        foregroundMask = fgMaskMOG > THRESH;
        backgroundMask = fgMaskMOG <= THRESH;
        score = pongDir(&momentum, &pt, height, width);
        if (!score) {   // still moving
            // blank canvas
            circ.setTo(Scalar(0, 0, 0));
            Rect ballRegion(pt.x - RADIUS, pt.y - RADIUS, 2 * RADIUS, 2 * RADIUS);
            ballFrame = circ(ballRegion);

            fgMaskMOG.setTo(Scalar(255, 255, 255), foregroundMask);     // clean up
            fgMaskMOG.setTo(Scalar(0, 0, 0), backgroundMask);
            handFrame = fgMaskMOG(ballRegion);

            int halfRad = RADIUS / 2;
            // top left
            small.x = halfRad;   small.y = halfRad;
            sum = getOverlap(&ballFrame, &handFrame, &small);
            momentum.x += pt.x < width / 2 ? sum : 0;
            momentum.y += sum;

            // top right
            small.x = 3 * halfRad;  small.y = halfRad;
            sum = getOverlap(&ballFrame, &handFrame, &small);
            momentum.x += pt.x >= width / 2 ? -sum : 0;
            momentum.y += sum;

            // bottom left
            small.x = halfRad;    small.y = 3 * halfRad;
            sum = getOverlap(&ballFrame, &handFrame, &small);
            momentum.x += pt.x < width / 2 ? sum : 0;
            momentum.y -= sum;

            // bottom right
            small.x = 3 * halfRad;   small.y = 3 * halfRad;
            sum = getOverlap(&ballFrame, &handFrame, &small);
            momentum.x += pt.x >= width / 2 ? -sum : 0;
            momentum.y -= sum;
        }
        else if (!timer--){
          // someone scored, so decrement timer and reset when timer = 0
          timer = 50;
          reset_board(&pt, &momentum, width, height);
          if (score > 0) left += 1;
          else          right += 1;

          // Zero out scoreFrame
          scoreFrame = scoreFrame > 256;

          // put score on the left
          putText(scoreFrame, to_string(left), Point(50-RADIUS,height-50),
                  FONT_HERSHEY_SCRIPT_SIMPLEX, 2, Scalar(255,255,255), 1, 8, false );
          // put right score on right
          putText(scoreFrame, to_string(right), Point(width-50,height-50),
                  FONT_HERSHEY_SCRIPT_SIMPLEX, 2, Scalar(255,255,255), 1, 8, false );

          //Draw the games dividing line
          line(scoreFrame, Point(width / 2,0), Point(width / 2,height), Scalar(0, 255, 0),
             2, 0, 0);

          if ( right >= 5 || left >= 5){
            // game over, stop playing
            game_over(right, left, scoreFrame, cap);
            break;
          }
        }
        // outFrame -= Scalar(50,50,50);
        // cvtColor(foregroundMask, inputFrame, CV_LOAD_IMAGE_COLOR);

        // outFrame.setTo(Scalar(0, 0, 0));
        // outFrame.setTo(Scalar(255, 255, 255), foregroundMask);

        //Set the color of the ball here.
        cvtColor(foregroundMask,foregroundMask ,COLOR_GRAY2BGR);
        addWeighted(outFrame, 0.75, foregroundMask, 0.25, 1, outFrame, -1);
        drawCircle(outFrame, pt, RADIUS, Scalar( 0, 0, 255 ));


        // Put score frame onto image
        addWeighted(outFrame, 0.75, scoreFrame, 1.0, 1, outFrame, -1);


        imshow(win,outFrame);

        // listening for key press
        char c = waitKey(30);
        if (c >= 0) {
            if (c == 'r') {
                pt.x = width / 2;
                pt.y = height / 2;
                continue;
            }
            else
                break;
        }
    }

    return 0;
}

void game_over(int right, int left) {
    waitKey(5000);
    exit(0);
}
