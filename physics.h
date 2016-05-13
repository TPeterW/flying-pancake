#ifndef PHYSICS_FP
#define PHYSICS_FP

#define RADIUS 24
#define THRESH 240.0f
#define ELASTICITY 7000

using namespace cv;
using namespace std;

// calculate the impulse on the ball
double getOverlap(Mat *, Mat *, Point *);

// draws the circle
void drawCircle(Mat, Point, int, Scalar);

// calculate the ball's direction to move
void calcDir(Point *momentum, Point *pt, int height, int width);

// calculate the pong ball's direction to move
int pongDir(Point *momentum, Point *pt, int height, int width);

void reset_board(Point * pt, Point * momentum, int width, int height);

#endif
