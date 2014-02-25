#include "affineTransform.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sys/stat.h>


Mat getTranslationMatrix(Point2f offset)
{
    Mat translation  = Mat::eye(Size(3,3), CV_64FC1);
    translation.at<double>(0, 2) = offset.x;
    translation.at<double>(1, 2) = offset.y;
    return translation;
}

Mat getScaleMatrix(double scaleFactor)
{
    Mat scale  = Mat::eye(Size(3,3), CV_64FC1);
    scale.at<double>(0, 0) = scaleFactor;
    scale.at<double>(0, 1) = 0.0;
    scale.at<double>(1, 0) = 0.0;
    scale.at<double>(1, 1) = scaleFactor;
    return scale;
}

Mat myGetRotationMatrix2D(Point2f center, double rotationAngle, double scaleFactor)
{
    Mat mat1 = Mat::eye(Size(3,3), CV_64FC1);
    Mat mat2 = Mat::eye(Size(3,3), CV_64FC1);
    Mat mat3 = Mat::eye(Size(3,3), CV_64FC1);
    Mat mat4 = Mat::eye(Size(3,3), CV_64FC1);
    
    // Move image to origin
    mat1.at<double>(0, 2) = -center.x;
    mat1.at<double>(1, 2) = -center.y;

    // Rotate image
    double theta = rotationAngle * M_PI / 180.0;
    mat2.at<double>(0, 0) = cos(theta);
    mat2.at<double>(0, 1) = sin(theta);
    mat2.at<double>(1, 0) = -sin(theta);
    mat2.at<double>(1, 1) = cos(theta);
    
    // Scale image
    mat3.at<double>(0, 0) = scaleFactor;
    mat3.at<double>(1, 1) = scaleFactor;
    
    // Move back to original location
    mat4.at<double>(0, 2) = center.x;
    mat4.at<double>(1, 2) = center.y;
    
    // Return composition of matrices
    return mat4 * (mat3 * (mat2 * mat1));
}

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
