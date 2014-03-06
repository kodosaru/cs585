//
//  File.cpp
//  Assignment6
//
//  Created by Don Johnson on 3/5/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#include "Functions.h"
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
#include <stdio.h>

using namespace cv;
using namespace std;

Mat createMask(Mat& image)
{
    Mat mask(image.size(),CV_8UC1);
    cvtColor(image,mask,CV_RGB2GRAY);
    threshold(mask,mask,0,255,THRESH_BINARY);
    return mask;
}

//Required: Compute the mosaic footprint and offset
//Assuming that the second image is warped with the homography, compute the total size of the
//footprint of the mosaic. You will also need to compute any potential offset that may occur if the
//second image is above or to the left of the first image
void computeMosaicFootprint(Mat& img1, Mat& img2, Mat& H, Size& mosaicSize, Point2f& mosaicOffset)
{
    //-- Get the corners from the img1
    std::vector<Point2f> img1_corners(4);
    img1_corners[0] = Point(0,0);
    img1_corners[1] = Point( img1.cols-1, 0 );
    img1_corners[2] = Point( img1.cols-1, img1.rows-1 );
    img1_corners[3] = Point( 0, img1.rows-1 );
    printf("img %d corners: (%f,%f),(%f,%f),(%f,%f),(%f,%f)\n",1,img1_corners[0].x,img1_corners[0].y,img1_corners[1].x,img1_corners[1].y,img1_corners[2].x,img1_corners[2].y,img1_corners[3].x,img1_corners[3].y);
    
    //-- Get the corners from the img2
    std::vector<Point2f> img2_corners(4);
    img2_corners[0] = Point(0,0);
    img2_corners[1] = Point( img2.cols-1, 0 );
    img2_corners[2] = Point( img2.cols-1, img2.rows-1 );
    img2_corners[3] = Point( 0, img2.rows-1 );
    printf("img %d corners: (%f,%f),(%f,%f),(%f,%f),(%f,%f)\n",2,img2_corners[0].x,img2_corners[0].y,img2_corners[1].x,img2_corners[1].y,img2_corners[2].x,img2_corners[2].y,img2_corners[3].x,img2_corners[3].y);
    
    // Transform corners from img2 using homography matrix
    std::vector<Point2f> img2_projected_corners(4);
    
    warpCorners(img2, H, img2_projected_corners);
    
    double xMin = DBL_MAX;
    double xMax = -DBL_MAX;
    double yMin = DBL_MAX;
    double yMax = -DBL_MAX;
    for(int i=0;i<4;i++)
    {
        xMin=img1_corners[i].x<xMin?img1_corners[i].x:xMin;
        xMax=img1_corners[i].x>xMax?img1_corners[i].x:xMax;
        yMin=img1_corners[i].y<yMin?img1_corners[i].y:yMin;
        yMax=img1_corners[i].y>yMax?img1_corners[i].y:yMax;
        
        xMin=img2_projected_corners[i].x<xMin?img2_projected_corners[i].x:xMin;
        xMax=img2_projected_corners[i].x>xMax?img2_projected_corners[i].x:xMax;
        yMin=img2_projected_corners[i].y<yMin?img2_projected_corners[i].y:yMin;
        yMax=img2_projected_corners[i].y>yMax?img2_projected_corners[i].y:yMax;
    }
    printf("xMin: %f, yMin: %f, xMax: %f, yMax: %f\n",xMin,yMin,xMax,yMax);
    if(xMax<0.0)
    {
        printf("xMax < 0 ERROR: %f",xMax);
        exit(1);
    }
    if(yMax<0.0)
    {
        printf("yMax < 0 ERROR: %f",yMax);
        exit(1);
    }
    mosaicOffset.x=xMin<0.0?-xMin:0.0;
    mosaicOffset.y=yMin<0.0?-yMin:0.0;
    mosaicSize.width=(int)(xMax-xMin+1.0);
    mosaicSize.height=(int)(yMax-yMin+1.0);
    printf("mosaic width: %d, height: %d\n",mosaicSize.width,mosaicSize.height);
}

void computeWarpedImageFootprint(Mat& img, Mat& H, Size& resultSize)
{
    //-- Get the corners from the img2
    std::vector<Point2f> img_corners(4);
    img_corners[0] = Point(0,0);
    img_corners[1] = Point( img.cols-1, 0 );
    img_corners[2] = Point( img.cols-1, img.rows-1 );
    img_corners[3] = Point( 0, img.rows-1 );
    printf("img corners: (%f,%f),(%f,%f),(%f,%f),(%f,%f)\n",img_corners[0].x,img_corners[0].y,img_corners[1].x,img_corners[1].y,img_corners[2].x,img_corners[2].y,img_corners[3].x,img_corners[3].y);
    
    // Transform corners from img2 using homography matrix
    std::vector<Point2f> img_projected_corners(4);
    
    warpCorners(img, H, img_projected_corners);
    
    double xMin = DBL_MAX;
    double xMax = -DBL_MAX;
    double yMin = DBL_MAX;
    double yMax = -DBL_MAX;
    for(int i=0;i<4;i++)
    {
        xMin=img_projected_corners[i].x<xMin?img_projected_corners[i].x:xMin;
        xMax=img_projected_corners[i].x>xMax?img_projected_corners[i].x:xMax;
        yMin=img_projected_corners[i].y<yMin?img_projected_corners[i].y:yMin;
        yMax=img_projected_corners[i].y>yMax?img_projected_corners[i].y:yMax;
    }
    printf("xMin: %f, yMin: %f, xMax: %f, yMax: %f\n",xMin,yMin,xMax,yMax);
    if(xMax<0.0)
    {
        printf("xMax < 0 ERROR: %f",xMax);
        exit(1);
    }
    if(yMax<0.0)
    {
        printf("yMax < 0 ERROR: %f",yMax);
        exit(1);
    }
    resultSize.width=(int)(ceil(xMax)-floor(xMin)+1.0);
    resultSize.height=(int)(ceil(yMax)-floor(yMin)+1.0);
    printf("result width: %d, height: %d\n",resultSize.width,resultSize.height);
}

//Required: Use the given homography to warp the corners of the image
//You may want to use this as a subroutine to compute the mosaic footprint
//http://docs.opencv.org/modules/core/doc/operations_on_arrays.html#perspectivetransform
void warpCorners(Mat& input_img, Mat& H, vector<Point2f>& imgCorners)
{
    // Get the corners from the img
    std::vector<Point2f> input_img_corners(4);
    input_img_corners[0] = Point(0.0,0.0);
    input_img_corners[1] = Point( input_img.cols-1.0, 0.0 );
    input_img_corners[2] = Point( input_img.cols-1.0, input_img.rows-1.0 );
    input_img_corners[3] = Point( 0.0, input_img.rows-1.0 );
    printf("input image corners: (%f,%f),(%f,%f),(%f,%f),(%f,%f)\n",input_img_corners[0].x,input_img_corners[0].y,input_img_corners[1].x,input_img_corners[1].y,input_img_corners[2].x,input_img_corners[2].y,input_img_corners[3].x,input_img_corners[3].y);
    
    // Warp corners
    perspectiveTransform( input_img_corners, imgCorners, H);
    
    // Print warped corner values
    printf("warped input image corners: (%f,%f),(%f,%f),(%f,%f),(%f,%f)\n",imgCorners[0].x,imgCorners[0].y,imgCorners[1].x,imgCorners[1].y,imgCorners[2].x,imgCorners[2].y,imgCorners[3].x,imgCorners[3].y);
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
        if( matches[i].distance <= max(min_dist*2, 0.02) )
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
    resize(img_matches,img_matches,Size(0,0),0.4,0.4,INTER_LINEAR);
    imshow( "Homography Matches", img_matches );
    
    
    //Compute the homography
    Mat H = findHomography(img2Points, img1Points, CV_RANSAC);
    return H;
}

