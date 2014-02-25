// Assignment 5, Part 1
// OpenCV has no built-in function to shear an image about a center, so we will make one

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
Mat getShearMatrix2D(Point2f center, double XShearFactor, double YShearFactor, double scaleFactor);


Mat original, result;

int main(int argc, char* argv[])
{
    String dataDir="/Users/donj/workspace/cs585/Lab5/Data/";
    original = imread(dataDir+argv[1]);

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
            imwrite(dataDir+"Assignment5_Part2_Output.png", result);
        }
    }
    return 0;
}

// Required
Mat getTranslationMatrix(Point2f offset)
{
    Mat translation  = Mat::eye(Size(3,3), CV_64FC1);
    translation.at<double>(0, 2) = offset.x;
    translation.at<double>(1, 2) = offset.y;
    return translation;
}

// Required
Mat getScaleMatrix(double scaleFactor)
{
    Mat scale  = Mat::eye(Size(3,3), CV_64FC1);
    scale.at<double>(0, 0) = scaleFactor;
    scale.at<double>(0, 1) = 0.0;
    scale.at<double>(1, 0) = 0.0;
    scale.at<double>(1, 1) = scaleFactor;
    return scale;
}

// Required
Mat getShearMatrix(double XShearFactor, double YShearFactor)
{
    Mat mat2 = Mat::eye(Size(3,3), CV_64FC1);
    mat2.at<double>(0, 1) = XShearFactor;
    mat2.at<double>(1, 0) = YShearFactor;
    return mat2;
}

// Required: Compose the basic, primitive matrices in order
// to perform a shearing operation about the center of the image
Mat getShearMatrix2D(Point2f center, double XShearFactor, double YShearFactor, double scaleFactor)
{
    Mat mat1 = Mat::eye(Size(3,3), CV_64FC1);
    Mat mat2 = Mat::eye(Size(3,3), CV_64FC1);
    Mat mat3 = Mat::eye(Size(3,3), CV_64FC1);
    Mat mat4 = Mat::eye(Size(3,3), CV_64FC1);
    
    // Move image to origin
    mat1.at<double>(0, 2) = -center.x;
    mat1.at<double>(1, 2) = -center.y;
    
    // Shear image
    mat2.at<double>(0, 1) = XShearFactor;
    mat2.at<double>(1, 0) = YShearFactor;
    
    // Scale image
    mat3.at<double>(0, 0) = scaleFactor;
    mat3.at<double>(1, 1) = scaleFactor;
    
    // Move back to original location
    mat4.at<double>(0, 2) = center.x;
    mat4.at<double>(1, 2) = center.y;
    
    // Return composition of matrices
    return mat4 * (mat3 * (mat2 * mat1));
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

