// Assignment 5, Part 1
// Reproduce the functionality of the OpenCV getRotationMatrix2D function from primitives
// The scale and rotation are controlled by slider. The center of rotation
// is set by clicking in the image
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


//One callback for both sliders
void onTrackbar(int trackbarValue, void* data);

//Callback for the mouse click
void onMouse(int event, int x, int y, int flags, void* data);

//Construct the basic matrices
Mat getScaleMatrix(double scaleFactor);
Mat getTranslationMatrix(Point2f offset);
Mat getRotationMatrix(double degrees);

// Required: Compose the basic matrices to reproduce the OpenCV functionality.
Mat myGetRotationMatrix2D(Point2f center, double rotationAngle, double scaleFactor);

Mat original, myResult, openCVResult;

int rotationAngle = 0;
int maxRotation = 360;
int scaleFactor = 10;
int maxScale = 20;
int centerX, centerY;

int main(int argc, char* argv[])
{
    original = imread(argv[1]);
    myResult.create(original.rows, original.cols, CV_8UC3);
    openCVResult.create(original.rows, original.cols, CV_8UC3);
    centerX = original.cols/2;
    centerY = original.rows/2;

    //Create windows for original image (with sliders for scale and rotation)
    namedWindow("Original", 1);
    createTrackbar("Angle", "Original", &rotationAngle, maxRotation, onTrackbar);
    createTrackbar("Scale", "Original", &scaleFactor, maxScale, onTrackbar);
    setMouseCallback("Original", onMouse);

    //Create windows for the OpenCV result and the hand-coded result
    namedWindow("My Result", 1);
    namedWindow("OpenCV Result", 1);

    //The work of transforming the image is done in the trackbar callback
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
            imwrite("Assignment5_Part1_Output_Mine.png", myResult);
            imwrite("Assignment5_Part1_Output_OpenCV.png", openCVResult);
        }
    }
    return 0;
}

// Callback for the track bar, which applies the transformation to the original image
// using both the OpenCV call and the hand-coded clone
void onTrackbar(int trackbarValue, void* data)
{
    double scale = 1+(scaleFactor-10.0)/10.0;

    //Our version
    Mat myTransform = myGetRotationMatrix2D(Point2f(centerX, centerY), rotationAngle, scale);
    myResult.setTo(Scalar(0));
    warpPerspective(original, myResult, myTransform, myResult.size(), INTER_CUBIC, BORDER_TRANSPARENT);

    //OpenCV version
    Mat openCVTransform = getRotationMatrix2D(Point2f(centerX, centerY), rotationAngle, scale);
    openCVResult.setTo(Scalar(0));
    warpAffine(original, openCVResult, openCVTransform, myResult.size(), INTER_CUBIC, BORDER_TRANSPARENT);
}

//Change the center of rotation with a mouse click
void onMouse(int event, int x, int y, int flags, void* data)
{
    if(event==EVENT_LBUTTONDOWN)
    {
        centerX = x;
        centerY = y;
        onTrackbar(-1, NULL);
    }
}

//Required: chain together primitive matrices to scale and rotate a matrix about a particular center of rotation
//Our version will return a matrix that is 3 x 3, which will be convenient for combining with subsequent operations
Mat myGetRotationMatrix2D(Point2f center, double rotationAngle, double scaleFactor)
{
}

//Required: construct a matrix to represent translation
Mat getTranslationMatrix(Point2f offset)
{
}

//Required: construct a matrix to represent scaling
Mat getScaleMatrix(double scaleFactor)
{
}

//Given: rotation
Mat getRotationMatrix(double degrees)
{
    double theta = degrees * M_PI / 180.0;
    Mat rotation  = Mat::eye(Size(3,3), CV_64FC1);
    rotation.at<double>(0, 0) = cos(theta);
    rotation.at<double>(0, 1) = sin(theta);
    rotation.at<double>(1, 0) = -sin(theta);
    rotation.at<double>(1, 1) = cos(theta);
    return rotation;    
}

