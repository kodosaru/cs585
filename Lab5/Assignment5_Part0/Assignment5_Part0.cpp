// Assignment 5, Part 0
// This program allows you to control the rotation and scaling of an image 
// using two sliders

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

//These are the callbacks for the two sliders
void onRotationTrackbar(int trackbarValue, void* data);
void onScaleTrackbar(int trackbarValue, void* data);

//Required
Mat getScaleMatrix(double scaleFactor);

//Given
Mat getRotationMatrix(double degrees);

Mat original, rotationResult, scaleResult;

int rotationAngle = 0;
int maxRotation = 360;
int scaleFactor = 10;
int maxScale = 20;

int main(int argc, char* argv[])
{
    original = imread(argv[1]);
    rotationResult.create(original.rows, original.cols, CV_8UC3);
    scaleResult.create(original.rows, original.cols, CV_8UC3);

    //Create windows for the original version as well as a scaled version and a rotated version
    //Windows for results have sliders to control the rotation and scaling
    namedWindow("Original", 1);
    namedWindow("Rotation Result", 1);
    createTrackbar("Angle", "Rotation Result", &rotationAngle, maxRotation, onRotationTrackbar);
    namedWindow("Scale Result", 1);
    createTrackbar("Scale", "Scale Result", &scaleFactor, maxScale, onScaleTrackbar);

    while( 1 == 1)
    {
        imshow("Original", original);
        imshow("Rotation Result", rotationResult);
        imshow("Scale Result", scaleResult);
        char key = waitKey(33);
        if(key == 'q')
        {
            break;
        }
        if(key == ' ')
        {
            imwrite("Assignment5_Part0_Output_Rotation.png", rotationResult);
            imwrite("Assignment5_Part0_Output_Scale.png", scaleResult);
        }
    }
    return 0;
}


void onRotationTrackbar(int trackbarValue, void* data)
{
    //Rotate the image using our hand-coded primitive rotation matrix (as in Lab)
    Mat transform = getRotationMatrix(rotationAngle);
    rotationResult.setTo(Scalar(0));
    warpPerspective(original, rotationResult, transform, rotationResult.size(), INTER_CUBIC, BORDER_TRANSPARENT);
}

void onScaleTrackbar(int trackbarValue, void* data)
{
    //Scale the image using a hand-coded primtive scaling matrix
    Mat transform = getScaleMatrix(1+(scaleFactor-10.0)/10.0);
    scaleResult.setTo(Scalar(0));
    warpPerspective(original, scaleResult, transform, scaleResult.size(), INTER_CUBIC, BORDER_TRANSPARENT);
}

//Required: Fill in the code to construct a matrix to apply a scale factor
Mat getScaleMatrix(double scaleFactor)
{
}

//Given: Rotation
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
