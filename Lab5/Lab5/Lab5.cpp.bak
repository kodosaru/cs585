// Lab 5
// We will work with image transformations, starting with rotation
// http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html
//
// This program allows you to control the rotation of an image using a slider
// The rotation is done two ways:
// 1.) Using the built-in OpenCV function to rotate about the center
// 2.) Using a home-grown rotation matrix (which will rotate about the origin (upper-left corner)

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


void onRotationTrackbar(int trackbarValue, void* data);
Mat getRotationMatrix(double degrees);

Mat original, myResult, openCVResult;

int rotationAngle = 0;
int maxRotation = 360;

int main(int argc, char* argv[])
{
    original = imread(argv[1]);
    myResult.create(original.rows, original.cols, CV_8UC3);
    openCVResult.create(original.rows, original.cols, CV_8UC3);

    namedWindow("Original", 1);
    namedWindow("My Result", 1);
    namedWindow("OpenCV Result", 1);

    //The real work of the program is done in the onTrackbar function in response to the slider value
    createTrackbar("Angle", "Original", &rotationAngle, maxRotation, onRotationTrackbar);

    while( 1 == 1)
    {
        imshow("Original", original);
        imshow("My Result", myResult);
        imshow("OpenCV Result", openCVResult);
        char key = waitKey(33);
        if(key == 'q')
        {
            break;
        }
        if(key == ' ')
        {
        }
    }
    return 0;
}

//This function responds to the slider, which changes the rotation angle
void onRotationTrackbar(int trackbarValue, void* data)
{
    //This gets our home-grown, hand-coded rotation matrix (defined below)
    //The resulting matrix will be 3 x 3
    Mat myTransform = getRotationMatrix(rotationAngle);

    //Clear the image to black
    myResult.setTo(Scalar(0));

    //The warpPerspective function uses a general 3 x 3 matrix to transform the image
    warpPerspective(original, myResult, myTransform, myResult.size(), INTER_CUBIC, BORDER_TRANSPARENT);

    //The OpenCV similarity transformation (with rotation, scaling, and translation)
    //actually returns a 2 x 3 matrix (instead of 3 x 3) (since a similarity transformation is a type of affine transformation)
    // The only drawback is that using this matrix for subsequent manipulations is a little inconvenient
    Mat openCVTransform = getRotationMatrix2D(Point2f(original.cols/2, original.rows/2), rotationAngle, 1);

    //Clear the image to black
    openCVResult.setTo(Scalar(0));

    //The warpAffine function uses a 2 x 3 matrix to transform an image
    warpAffine(original, openCVResult, openCVTransform, openCVResult.size(), INTER_CUBIC, BORDER_TRANSPARENT);
}

//This function constructs a plain, primitive rotation matrix
Mat getRotationMatrix(double degrees)
{
    //convert degrees to radians
    double theta = degrees * M_PI / 180.0;

    //create a new 3 x 3 identity matrix of type double
    Mat rotation  = Mat::eye(Size(3,3), CV_64FC1);

    //Set the coefficients of the upper-left 2x2
    //The matrix is a little different from the version in the class notes
    //since the Y axis of images is positive down
    //(THe matrix below matches the OpenCV convention)
    rotation.at<double>(0, 0) = cos(theta);
    rotation.at<double>(0, 1) = sin(theta);
    rotation.at<double>(1, 0) = -sin(theta);
    rotation.at<double>(1, 1) = cos(theta);
    return rotation;    
}
