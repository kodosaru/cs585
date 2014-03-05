// Assignment 6, Do the mosaic creation pipeline for two images

#include "stdafx.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include <string>
#include <vector>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace cv;
using namespace std;

//http://docs.opencv.org/doc/tutorials/features2d/feature_homography/feature_homography.html
//http://docs.opencv.org/trunk/doc/py_tutorials/py_feature2d/py_orb/py_orb.html

Mat getTranslationMatrix(Point2f offset);
Mat getHomography(Mat& img1, Mat& img2);
void warpCorners(Mat& img, Mat& H, vector<Point2f>& warped);
void computeMosaicFootprint(Mat& img1, Mat& img2, Mat& H, Size& mosaicSize, Point2f& mosaicOffset);
void composeMosaic_v1(Mat& img1, Mat& img2, Mat& H, Size& mosaicSize, Point2f& mosaicOffset, Mat& result);
void composeMosaic_v2(Mat& img1, Mat& img2, Mat& H, Size& mosaicSize, Point2f& mosaicOffset, Mat& result);


int main(int argc, char* argv[])
{
    Mat img1, img2, mask;
    img1 = imread(argv[1]);
    img2 = imread(argv[2]);
    mask = Mat::ones(img1.rows, img1.cols, CV_8UC1);

    Mat H = getHomography(img1, img2);

    //mess with the color so we can see the seam
    cvtColor(img1, img1, CV_BGR2RGB);

    Size mosaicSize;
    Point2f mosaicOffset;
    computeMosaicFootprint(img1, img2, H, mosaicSize, mosaicOffset);


    Mat result1;
    composeMosaic_v1(img1, img2, H, mosaicSize, mosaicOffset, result1);
    imwrite("Assignment6_Part1_Result1.png", result1);
    imshow("result1", result1);
    waitKey(0);

    Mat result2;
    composeMosaic_v2(img1, img2, H, mosaicSize, mosaicOffset, result2);
    imwrite("Assignment6_Part1_Result2.png", result2);
    imshow("result2", result2);
    waitKey(0);
    int i=0;
}

//Required: Given the two images, the homography between them, and the mosaic footprint information,
//compose the mosaic with a simple "copy and paste" operation (using warpPerspective)
void composeMosaic_v1(Mat& img1, Mat& img2, Mat& H, Size& mosaicSize, Point2f& mosaicOffset, Mat& result)
{
}

//Required: Given the two images, the homography between them, and the mosaic footprint information,
//compose the mosaic by choosing the source image that is closest to the output pixel.
//For my solution, I used the "distanceTransform" function, but you don't have to. 
//http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#distancetransform
// You will probably need to iterate over all of the pixels in the output manually
void composeMosaic_v2(Mat& img1, Mat& img2, Mat& H, Size& mosaicSize, Point2f& mosaicOffset, Mat& result)
{
}


//Required: Compute the mosaic footprint and offset
//Assuming that the second image is warped with the homography, compute the total size of the 
//footprint of the mosaic. You will also need to compute any potential offset that may occur if the 
//second image is above or to the left of the first image
void computeMosaicFootprint(Mat& img1, Mat& img2, Mat& H, Size& mosaicSize, Point2f& mosaicOffset)
{
}


//Required: Use the given homography to warp the corners of the image
//You may want to use this as a subroutine to compute the mosaic footprint
//http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#perspectivetransform
void warpCorners(Mat& img, Mat& H, vector<Point2f>& imgCorners)
{
}

//Given: construct a matrix to represent translation
Mat getTranslationMatrix(Point2f offset)
{
    Mat translation = Mat::eye(Size(3,3), CV_64FC1);
    translation.at<double>(0, 2) = offset.x;
    translation.at<double>(1, 2) = offset.y;
    return translation;
}

// Given: Compute a homography between two images
//mostly copied from: http://docs.opencv.org/doc/tutorials/features2d/feature_homography/feature_homography.html
Mat getHomography(Mat& img1, Mat& img2)
{
    Mat img1BW, img2BW;
    cvtColor(img1, img1BW, CV_BGR2GRAY);
    cvtColor(img2, img2BW, CV_BGR2GRAY);
    GaussianBlur(img1BW, img1BW, Size(5,5), 1);
    GaussianBlur(img2BW, img2BW, Size(5,5), 1);

    //-- Step 1: Find the keypoints
    int minHessian = 400;
    Ptr<FeatureDetector> detector = new SurfFeatureDetector(1, true);
    std::vector<KeyPoint> keypoints_1, keypoints_2;

    detector->detect( img1BW, keypoints_1 );
    detector->detect( img2BW, keypoints_2 );

    //-- Step 2: Calculate descriptors (feature vectors)
    Ptr<DescriptorExtractor> extractor = new SurfDescriptorExtractor;
    Mat descriptors_1, descriptors_2;

    extractor->compute( img1BW, keypoints_1, descriptors_1 );
    extractor->compute( img2BW, keypoints_2, descriptors_2 );

    //-- Step 3: Matching descriptor vectors using FLANN matcher
    BFMatcher matcher;
    std::vector< DMatch > matches;
    vector<Point2f> img1Points;
    vector<Point2f> img2Points;

    matcher.match( descriptors_1, descriptors_2, matches );

    //-- Step 4: Use some heuristics to prune out bad matches
    double max_dist = 0; 
    double min_dist = 1000;
    for( int i = 0; i < descriptors_1.rows; i++ )
    { double dist = matches[i].distance;
    if( dist < min_dist ) min_dist = dist;
    if( dist > max_dist ) max_dist = dist;
    }

    std::vector< DMatch > good_matches;

    for( int i = 0; i < descriptors_1.rows; i++ )
    { 
        if( matches[i].distance <= max(min_dist*3, 0.02) )
        { 
            good_matches.push_back( matches[i]); 

            //store the good matches in two vectors that we will use to find the homography
            img1Points.push_back(keypoints_1[matches[i].queryIdx].pt);
            img2Points.push_back(keypoints_2[matches[i].trainIdx].pt);
        }
    }

    //-- Draw only "good" matches
    Mat img_matches;
    drawMatches( img1, keypoints_1, img2, keypoints_2,
        good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
        vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    //-- Show detected matches
    imshow( "Homography Matches", img_matches );


    //Compute the homography
    Mat H = findHomography(img2Points, img1Points, CV_RANSAC);
    return H;
}

