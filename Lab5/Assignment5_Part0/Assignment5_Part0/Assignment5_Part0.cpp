// Assignment 5, Part 0
// This program allows you to control the rotation and scaling of an image 
// using two sliders

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <cmath>
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

void printTransformMat(Mat rm)
{
    cout << "Transform matrix:" << endl;
    cout << rm.at<double>(0,0) << "\t" <<  rm.at<double>(0,1) << "\t" <<  rm.at<double>(0,2) << endl;
    cout << rm.at<double>(1,0) << "\t" <<  rm.at<double>(1,1) << "\t" <<  rm.at<double>(1,2) << endl;
}


//Required
Mat getScaleMatrix(double scaleFactor);
Mat getTranslationMatrix(double tx, double ty);

//Given
Mat getRotationMatrix(double degrees, Point center);

Mat original, rotationResult, scaleResult;

int rotationAngle = 0;
int maxRotation = 360;
int scaleFactor = 10;
int maxScale = 20;

int main(int argc, char* argv[])
{
    String dataDir="/Users/donj/workspace/cs585/Lab5/Data/";
    original = imread(dataDir+argv[1]);
    rotationResult.create(original.rows, original.cols, CV_8UC3);
    scaleResult.create(original.rows, original.cols, CV_8UC3);
    
    //Create windows for the original version as well as a scaled version and a rotated version
    //Windows for results have sliders to control the rotation and scaling
    namedWindow("Original", 1);
    namedWindow("Rotation Result", 1);
    createTrackbar("Angle", "Rotation Result", &rotationAngle, maxRotation, onRotationTrackbar);
    namedWindow("Scale Result", CV_WINDOW_AUTOSIZE);
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
            imwrite(dataDir+"Assignment5_Part0_Output_Rotation.png", rotationResult);
            imwrite(dataDir+"Assignment5_Part0_Output_Scale.png", scaleResult);
        }
    }
    return 0;
}


void onRotationTrackbar(int trackbarValue, void* data)
{
    Mat transform, tempResult;
    Point size=original.size();
    
    // Calculate larger destination based on image diagonal to allow for rotation
    double diag=(int)(sqrt(pow(size.x,2) + pow(size.y,2))+0.5);
    size=Point(diag,diag);
    Point center(diag/2.0,diag/2.0);
    tempResult.create(size, CV_8UC3);
    tempResult.setTo(Scalar(0));
    double tx=center.x-original.cols/2;
    double ty=center.y-original.rows/2;
    
    // Translate orginal image center to that of larger destination image
    transform=getTranslationMatrix(tx,ty);
    printTransformMat(transform);
    warpPerspective(original, tempResult, transform, size, INTER_CUBIC, BORDER_TRANSPARENT);
    
    // Rotate the image about center
    transform = getRotationMatrix(-rotationAngle, center);
    printTransformMat(transform);
    rotationResult.setTo(Scalar(0));
    warpPerspective(tempResult, rotationResult, transform, size, INTER_CUBIC, BORDER_TRANSPARENT);
}

void onScaleTrackbar(int trackbarValue, void* data)
{
    //Scale the image using a hand-coded primtive scaling matrix
    double temp=1+(scaleFactor-10.0)/10.0;
    Mat transform = getScaleMatrix(temp);
    printTransformMat(transform);

    scaleResult.release();
    scaleResult.create((int)(temp*original.rows+0.5), (int)(temp*original.cols+0.5), CV_8UC3);
    scaleResult.setTo(Scalar(0));
    warpPerspective(original, scaleResult, transform, scaleResult.size(), INTER_CUBIC, BORDER_TRANSPARENT);
    cout<<"scaleFactor: "<<temp<<endl;
    cout<<"Dim x: "<<scaleResult.cols<<endl;
    cout<<"Dim y: "<<scaleResult.rows<<endl;
}

Mat getTranslationMatrix(double tx, double ty)
{
    //scaleFactor=1.0;
    Mat translation  = Mat::eye(Size(3,3), CV_64FC1);
    cout << "(tx, ty): (" << tx << "," << ty << ")" << endl;
    translation.at<double>(0, 2) = tx;
    translation.at<double>(1, 2) = ty;
    
    return translation;
}

//Required: Fill in the code to construct a matrix to apply a scale factor
Mat getScaleMatrix(double scaleFactor)
{
    //scaleFactor=1.0;
    Mat scale  = Mat::eye(Size(3,3), CV_64FC1);
    cout << "Scale factor: " << scaleFactor << endl;
    scale.at<double>(0, 0) = scaleFactor;
    scale.at<double>(0, 1) = 0.0;
    scale.at<double>(1, 0) = 0.0;
    scale.at<double>(1, 1) = scaleFactor;
    return scale;
}

//Given: Rotation
Mat getRotationMatrix(double degrees, Point center)
{
    double theta = degrees * M_PI / 180.0;
    Mat rotation  = Mat::eye(Size(3,3), CV_64FC1);
    rotation.at<double>(0, 0) = cos(theta);
    rotation.at<double>(0, 1) = sin(theta);
    rotation.at<double>(0, 2) = (1.0-cos(theta)) * center.x - sin(theta) * center.y;
    rotation.at<double>(1, 0) = -sin(theta);
    rotation.at<double>(1, 1) = cos(theta);
    rotation.at<double>(1, 2) = sin(theta) * center.x + (1.0-cos(theta)) * center.y;
    return rotation;    
}
