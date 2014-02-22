// Create a spring-time collage of images to encourage spring to come sooner
// Here is a basic skeleton. Fill it in any way that suits you to create your composition
// Use any pieces of any labs and homeworks that you need
#include "stdafx.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <string>
#include <vector>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace cv;
using namespace std;

//Suggestions only
Mat getScaleMatrix(double scaleFactor);
Mat getRotationMatrix(double degrees);
Mat getTranslationMatrix(Point2f offset);
Mat myGetRotationMatrix2D(Point2f center, double rotationAngle, double scaleFactor);
Mat getShearMatrix2D(Point2f& center, double XShearFactor, double YShearFactor, double scaleFactor);

//Suggestion only
vector<Mat> originals;
Mat result;

//For reference
//http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html
int main(int argc, char* argv[])
{
    for(int i=1; i<argc; i++)
    {
        originals.push_back(imread(argv[i]));
    }
    result.create(750, 750, CV_8UC3);

    namedWindow("Result", 1);

    while( 1 == 1)
    {
        imshow("Result", result);
        char key = waitKey(33);
        if(key == 'q')
        {
            break;
        }
        if(key == ' ')
        {
            imwrite("Assignment5_Output.png", result);
        }
    }
    return 0;
}

