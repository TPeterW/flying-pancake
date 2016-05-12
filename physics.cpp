#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "physics.h"
#include <stdlib.h>

double getOverlap(Mat *ballFrame, Mat *handFrame, Point *center)
{
    ballFrame->setTo(Scalar(0, 0, 0));
    drawCircle(*ballFrame, *center, RADIUS / 2, Scalar(255,255,255));

    bitwise_and(*ballFrame, *handFrame, *ballFrame);

    return sum(*ballFrame)[0] / ELASTICITY;
}

void drawCircle(Mat img, Point center, int radius, Scalar color)
{
    int thickness = -1;
    int lineType = 8;

    circle( img,
            center,
            radius,
            color,
            thickness,
            lineType);
}

void calcDir(Point *momentum, Point *pt, int height, int width)
{
      pt->x += momentum->x;
      pt->y += momentum->y;

      if (pt->y < height) {
           // Accelerate due to Gravity
          momentum->y += 3;
      } else {
          // Bounce on the bottom
          momentum->x = momentum->x * 0.9;
          momentum->y = -(momentum->y * .6);   // bounce back up and halt it
          pt->y = height;
      }

      // off the top
      if (pt->y < RADIUS) {
            pt->y = RADIUS;
            momentum->y = -(momentum->y * .6);
            momentum->x = momentum->x * .9;
      }
      // bounce on the Right
      if (pt->x > width) {
            pt->x=width;
            momentum->x = -(momentum->x * .6);
            momentum->y = momentum->y * .9;
      }
      // bounce on the Left
      if (pt->x < RADIUS) {
            pt->x = RADIUS;
            momentum->x = -(momentum->x * .6);
            momentum->y = momentum->y * .9;
      }
      // slow to stop if low momentum
      if (momentum->y * momentum->y <= 25 && pt->y == height){
          momentum->y = momentum->y / (2);
          momentum->x = momentum->x / (2);
      }
      momentum->y = momentum->y * 0.95;
      momentum->x = momentum->x * 0.95;
}

/***
 * returns 0 if nobody wins
 *        -1 if ball touches left wall
 *         1 if ball touches right wall
 */
int pongDir(Point *momentum, Point *pt, int height, int width)
{
    // move the ball first
    pt->x += momentum->x;
    pt->y += momentum->y;

    // stuck on the Right
    if (pt->x >= width - RADIUS) {
        pt->x = width - RADIUS;
        momentum->x = 0;
        momentum->y = 0;
        return 1;   // left wins
    }

    // stuck on the Left
    if (pt->x <= RADIUS) {
        pt->x = RADIUS;
        momentum->x = 0;
        momentum->y = 0;
        return -1;  // right wins
    }

    // off the top
    if (pt->y < RADIUS) {
        pt->y = RADIUS;
        momentum->x = momentum->x * 1.05;
        momentum->y = -(momentum->y * 1.05);
        return 0;
    }

    if (pt->y < height - RADIUS) {
        // Accelerate due to Gravity
        momentum->y += 0;
    }
    else {
        pt->y = height - RADIUS;     // make sure at bottom
        // Bounce on the bottom
        momentum->x = momentum->x * 1.05;
        momentum->y = -(momentum->y * 1.05);   // bounce back up
        return 0;
    }

    if (momentum->x*momentum->x + momentum->y*momentum->y > 200){
      momentum->y = momentum->y * 0.98;
      momentum->x = momentum->x * 0.98;
  }

    return 0;
}
void reset_board(Point * pt, Point * momentum, int width, int height){
  //This will take in a pointer to the point and a pointer to momentum
  //Will set the ball in the middle, and randomly give a momentum

      pt->x = width / 2;
      pt->y = height / 2;

      momentum->x = rand()%20 -10;
      momentum->y = rand()%20 -10;
}
