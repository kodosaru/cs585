// Assignment 5, Part 1
// OpenCV has no built-in function to shear an image about a center, so we will make one

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

using namespace cv;
using namespace std;

int xshear=100;
int yshear=100;
int maxShear = 200;
void onTrackbar(int value, void* data);

// Required: all the primitives
Mat getShearMatrix(double XShearFactor, double YShearFactor);
Mat getScaleMatrix(double scaleFactor);
Mat getTranslationMatrix(Point2f offset);
Mat getShearMatrix2D(Point2f& center, double XShearFactor, double YShearFactor, double scaleFactor);


Mat original, result;

int main(int argc, char* argv[])
{
    original = imread(argv[1]);

    //the result image for shearing will be larger so we can see the shape
    result.create(original.rows*1.5, original.cols*1.5, CV_8UC3);

    namedWindow("Original", 1);

    namedWindow("Result", 1);
    //separate x and y shear
    createTrackbar("X shear", "Result", &xshear, maxShear, onTrackbar);
    createTrackbar("Y shear", "Result", &yshear, maxShear, onTrackbar);

    while( 1 == 1)
    {
        imshow("Original", original);
        imshow("Result", result);
        char key = waitKey(33);
        if(key == 'q')
        {
            break;
        }
        if(key == ' ')
        {
            imwrite("Assignment5_Part2_Output.png", result);
        }
    }
    return 0;
}

// Required
Mat getTranslationMatrix(Point2f offset)
{
}

// Required
Mat getScaleMatrix(double scaleFactor)
{
}

// Required
Mat getShearMatrix(double XShearFactor, double YShearFactor)
{
}

// Required: Compose the basic, primitive matrices in order
// to perform a shearing operation about the center of the image
Mat getShearMatrix2D(Point2f& center, double XShearFactor, double YShearFactor, double scaleFactor)
{
}

//Construct and apply the shearing transformation to the image
void onTrackbar(int trackbarValue, void* data)
{
    double xshearVal = (double)(xshear-100)/100.0;
    double yshearVal = (double)(yshear-100)/100.0;
    // Shear the matrix about the center
    Mat shear = getShearMatrix2D(Point2f(original.cols/2, original.rows/2), xshearVal, yshearVal, 1);

    // Apply an extra translation to put the result at the center of the result image
    Mat translation = getTranslationMatrix(Point(result.cols/2 - original.cols/2, result.rows/2 - original.rows/2));
    Mat transform = translation * shear;

    // warp the image
    result.setTo(Scalar(0));
    warpPerspective(original, result, transform, result.size(), INTER_CUBIC, BORDER_TRANSPARENT);
}

