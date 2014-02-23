//
//  main.cpp
//  warpAffine
//
//  Created by Don Johnson on 2/23/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

/// Global variables
char* source_window = "Source image";
char* warp_window = "Warp";
char* warp_rotate_window = "Warp + Rotate";

void printRotMat(Mat rm)
{
    cout << "Rotation matrix:" << endl;
    cout << rm.at<float>(0,0) << "\t" <<  rm.at<float>(0,1) << "\t" <<  rm.at<float>(0,2) << endl;
    cout << rm.at<float>(1,0) << "\t" <<  rm.at<float>(1,1) << "\t" <<  rm.at<float>(1,2) << endl;
}

/** @function main */
int main( int argc, char** argv )
{
    String dataDir="/Users/donj/workspace/cs585/Lab5/Data/";
    Point2f srcTri[3];
    Point2f dstTri[3];
    
    //Mat rot_mat( 3, 3, CV_32FC1 );
     Mat rot_mat  = Mat::eye(Size(3,3), CV_64FC1);
    Mat warp_mat( 2, 3, CV_32FC1 );
    Mat src, warp_dst, warp_rotate_dst,final_dst;
    
    /// Load the image
    src = imread( dataDir+argv[1], 1 );
    
    /// Set the dst image the same type and size as src
    warp_dst = Mat::zeros( src.rows, src.cols, src.type() );
    final_dst = Mat::zeros( src.rows, src.cols, src.type() );
    
    /// Set your 3 points to calculate the  Affine Transform
    srcTri[0] = Point2f( 0,0 );
    srcTri[1] = Point2f( src.cols - 1, 0 );
    srcTri[2] = Point2f( 0, src.rows - 1 );
    
    dstTri[0] = Point2f( src.cols*0.0, src.rows*0.33 );
    dstTri[1] = Point2f( src.cols*0.85, src.rows*0.25 );
    dstTri[2] = Point2f( src.cols*0.15, src.rows*0.7 );
    
    /// Get the Affine Transform
    //warp_mat = getAffineTransform( srcTri, dstTri );
    
    Point newSize(src.cols,src.rows);
    
    /// Apply the Affine Transform just found to the src image
    //warpAffine( src, warp_dst, warp_mat, warp_dst.size() );
    
    /** Rotating the image after Warp */
    
    /// Compute a rotation matrix with respect to the center of the image
    //Point center = Point( warp_dst.cols/2, warp_dst.rows/2 );
    //Point center = Point( src.cols/2, src.rows/2 );
    //Point center = Point( src.cols/2, src.rows/2 );
    Point center = Point( 0, 0 );
    //double angle = -50.0;
    double angle = 0.0;
    //double scale = 0.6;
    double scale = 1.0;
    
    /// Get the rotation matrix with the specifications above
    //rot_mat = getRotationMatrix2D( center, angle, scale );
    printRotMat(rot_mat);
    rot_mat.at<float>(0,0)=1.0;
    rot_mat.at<float>(0,1)=0.0;
    rot_mat.at<float>(0,2)=0.0;
    rot_mat.at<float>(1,0)=0.0;
    rot_mat.at<float>(1,1)=1.0;
    rot_mat.at<float>(1,2)=0.0;
    double degrees = 45.0;
    double theta = degrees * M_PI / 180.0;
    Mat rotation  = Mat::eye(Size(3,3), CV_64FC1);
    rotation.at<double>(0, 0) = cos(theta);
    rotation.at<double>(0, 1) = sin(theta);
    rotation.at<double>(1, 0) = -sin(theta);
    rotation.at<double>(1, 1) = cos(theta);
    rotation.at<float>(0, 0) = 1.0;
    rotation.at<float>(0, 1) = 0.0;
    rotation.at<float>(1, 0) = 0.0;
    rotation.at<float>(1, 1) = 1.0;
    //rot_mat.at<float>(0,2)=src.cols;
    //rot_mat.at<float>(1,2)=src.rows;
    //c=rot_mat.at<float>(0,2);
    //f=rot_mat.at<float>(1,2);
    
    
    /// Rotate the warped image
    //warpAffine( warp_dst, warp_rotate_dst, rot_mat, warp_dst.size() );

    //warpAffine( src, warp_rotate_dst, rot_mat, newSize );
    warpPerspective(src, warp_rotate_dst, rotation, newSize, INTER_CUBIC, BORDER_TRANSPARENT);
    //invertAffineTransform(rot_mat, rot_mat);

    
    //rot_mat = getRotationMatrix2D( Point(0.0,0.0), 180.0, 1.0 );
    //printRotMat(rot_mat);    
    //warpAffine( warp_rotate_dst, final_dst, rot_mat, newSize );
    
    
    /// Show what you got
    namedWindow( source_window, CV_WINDOW_AUTOSIZE );
    imshow( source_window, src );
    
    //namedWindow( warp_window, CV_WINDOW_AUTOSIZE );
    //imshow( warp_window, warp_dst );
    
    namedWindow( warp_rotate_window, CV_WINDOW_AUTOSIZE );
    imshow( warp_rotate_window, warp_rotate_dst );
    //imshow( warp_rotate_window, final_dst );
    
    /// Wait until user exits the program
    waitKey(0);
    
    return 0;
}