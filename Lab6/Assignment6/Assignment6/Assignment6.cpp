// Assignment 6, Do the mosaic creation pipeline for two images

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
#include "Functions.h"

using namespace cv;
using namespace std;

//http://docs.opencv.org/doc/tutorials/features2d/feature_homography/feature_homography.html
//http://docs.opencv.org/trunk/doc/py_tutorials/py_feature2d/py_orb/py_orb.html

void composeMosaic_v1(Mat& img1, Mat& img2, Mat& H, Size& mosaicSize, Point2f& mosaicOffset, Mat& result);
void composeMosaic_v2(Mat& img1, Mat& img2, Mat& H, Size& mosaicSize, Point2f& mosaicOffset, Mat& result);
void composeMosaic_v2B(Mat& img1, Mat& img2, Mat& H, Size& mosaicSize, Point2f& mosaicOffset, Mat& result);

int main(int argc, char* argv[])
{
    String dataDir="/Users/donj/workspace/cs585/Lab6/Data/";
    vector<Mat> img;
    int nImages=0;
    for(int i=0;i<nImages;i++)
    {
        img.push_back(imread(dataDir+argv[i+2]));
    }
    
    Mat mask = Mat::ones(img[0].rows, img[0].cols, CV_8UC1);

    Mat H = getHomography(img[0], img[1]);

    //mess with the color so we can see the seam
    cvtColor(img[0], img[0], CV_BGR2RGB);

    Size mosaicSize;
    Point2f mosaicOffset;
    computeMosaicFootprint(img[0], img[1], H, mosaicSize, mosaicOffset);


    Mat result1;
    composeMosaic_v1(img[0], img[1], H, mosaicSize, mosaicOffset, result1);
    imwrite("Assignment6_Part1_Result1.png", result1);
    imshow("result1", result1);
    waitKey(0);

    Mat result2;
    composeMosaic_v2(img[0], img[1], H, mosaicSize, mosaicOffset, result2);
    imwrite("Assignment6_Part1_Result2.png", result2);
    imshow("result2", result2);
    waitKey(0);
    
    Mat result3;
    composeMosaic_v2B(img[0], img[1], H, mosaicSize, mosaicOffset, result3);
    imwrite("Assignment6_Part1_Result3.png", result3);
    imshow("result3", result3);
    waitKey(0);
    
    
}

//Required: Given the two images, the homography between them, and the mosaic footprint information,
//compose the mosaic with a simple "copy and paste" operation (using warpPerspective)
void composeMosaic_v1(Mat& img1, Mat& img2, Mat& H, Size& mosaicSize, Point2f& mosaicOffset, Mat& result)
{
    result.create(mosaicSize,CV_8UC3);
    result.setTo(Scalar(0,0,0));

    Mat TM = getTranslationMatrix(mosaicOffset);
    
    warpPerspective(img2, result, TM*H, result.size(), INTER_CUBIC, BORDER_TRANSPARENT);
    warpPerspective(img1, result, TM, result.size(), INTER_CUBIC, BORDER_TRANSPARENT);
}

inline bool isNonZeroPixel(Mat& img, int i, int j)
{
    if((img.data[img.step[0]*i + img.step[1]* j + 0] +
        img.data[img.step[0]*i + img.step[1]* j + 1] +
        img.data[img.step[0]*i + img.step[1]* j + 2]) > 0)
    {
        return true;
    }
    else{
        return false;
    }
}

//Required: Given the two images, the homography between them, and the mosaic footprint information,
//compose the mosaic by choosing the source image that is closest to the output pixel.
//For my solution, I used the "distanceTransform" function, but you don't have to. 
//http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#distancetransform
// You will probably need to iterate over all of the pixels in the output manually
void composeMosaic_v2(Mat& img1, Mat& img2, Mat& H, Size& mosaicSize, Point2f& mosaicOffset, Mat& result)
{
    result.create(mosaicSize,CV_8UC3);
    result.setTo(Scalar(0,0,0));
    Mat result1(mosaicSize,CV_8UC3);
    result1.setTo(Scalar(0,0,0));
    Mat result2(mosaicSize,CV_8UC3);
    result2.setTo(Scalar(0,0,0));
    
    Mat TM = getTranslationMatrix(mosaicOffset);
    
    warpPerspective(img1, result1, TM, result1.size(), INTER_CUBIC, BORDER_TRANSPARENT);
    warpPerspective(img2, result2, TM*H, result2.size(), INTER_CUBIC, BORDER_TRANSPARENT);
    
    Mat result1BW, result2BW;
    cvtColor(result1, result1BW, CV_BGR2GRAY);
    cvtColor(result2, result2BW, CV_BGR2GRAY);
    Mat result1Dist(result1.size(),CV_32FC1);
    Mat result2Dist(result2.size(),CV_32FC1);
    distanceTransform(result1BW,result1Dist,CV_DIST_L2 /*Euclidean*/,3);
    distanceTransform(result2BW,result2Dist,CV_DIST_L2 /*Euclidean*/,3);
    
    // Idea: Of multiple candidate pixels in the one closet to the nearest zero pixel (the border) is the one that is farthest away from the center. Consequently, choose the other candidate to meet this assignment requirement.
    float dist1,dist2;
    for(int i=0; i<result.rows; i++){
        for(int j=0; j<result.cols; j++){
                dist1 = result1Dist.data[result1Dist.step[0]*i + result1Dist.step[1]* j];
                dist2 = result2Dist.data[result2Dist.step[0]*i + result2Dist.step[1]* j];
            if(dist1 > dist2)
            {
                if(isNonZeroPixel(result1, i, j))
                {
                    printf("IMG1 Closest to center/Farthest from border - Point(%d,%d) img1: %0.2f img2: %0.2f\n",i,j,dist1,dist2);
                    result.data[result.step[0]*i + result.step[1]* j + 0] = result1.data[result1.step[0]*i + result1.step[1]* j + 0];
                    result.data[result.step[0]*i + result.step[1]* j + 1] = result1.data[result1.step[0]*i + result1.step[1]* j + 1];
                    result.data[result.step[0]*i + result.step[1]* j + 2] = result1.data[result1.step[0]*i + result1.step[1]* j + 2];
                }
                
            }
            else
            {
                if(isNonZeroPixel(result2, i, j))
                {
                    printf("IMG2 Closest to center/Farthest from border - Point(%d,%d) img1: %0.2f img2: %0.2f\n",i,j,dist1,dist2);
                    result.data[result.step[0]*i + result.step[1]* j + 0] = result2.data[result2.step[0]*i + result2.step[1]* j + 0];
                    result.data[result.step[0]*i + result.step[1]* j + 1] = result2.data[result2.step[0]*i + result2.step[1]* j + 1];
                    result.data[result.step[0]*i + result.step[1]* j + 2] = result2.data[result2.step[0]*i + result2.step[1]* j + 2];
                }

            }
        }
    }
}

void composeMosaic_v2B(Mat& img1, Mat& img2, Mat& H, Size& mosaicSize, Point2f& mosaicOffset, Mat& result)
{
    result.create(mosaicSize,CV_8UC3);
    result.setTo(Scalar(0,0,0));
    Mat result1(mosaicSize,CV_8UC3);
    result1.setTo(Scalar(0,0,0));
    Mat result2(mosaicSize,CV_8UC3);
    result2.setTo(Scalar(0,0,0));
    
    Mat TM = getTranslationMatrix(mosaicOffset);
    
    warpPerspective(img1, result1, TM, result1.size(), INTER_CUBIC, BORDER_TRANSPARENT);
    warpPerspective(img2, result2, TM*H, result2.size(), INTER_CUBIC, BORDER_TRANSPARENT);
    
    // Create masks
    Mat mask_result1(result1.size(),CV_8UC1);
    cvtColor(result1,mask_result1,CV_RGB2GRAY);
    threshold(mask_result1,mask_result1,0,255,THRESH_BINARY);
    Mat mask_result2(result2.size(),CV_8UC1);
    cvtColor(result2,mask_result2,CV_RGB2GRAY);
    threshold(mask_result2,mask_result2,0,255,THRESH_BINARY);
    
    // Find centroids
    Moments m1=moments(mask_result1, false);
    Moments m2=moments(mask_result2, false);
    double img1XCenter = m1.m10/m1.m00;
    double img1YCenter = m1.m01/m1.m00;
    printf("img1 centroid: (%0.2f,%0.2f)\n",img1XCenter,img1YCenter);
    double img2XCenter = m2.m10/m2.m00;
    double img2YCenter = m2.m01/m2.m00;
    printf("img2 centroid: (%0.2f,%0.2f)\n",img2XCenter,img2YCenter);

    
    float dist1,dist2;
    for(int i=0; i<result.rows; i++)
    {
        for(int j=0; j<result.cols; j++)
        {
            dist1 = sqrt(pow(img1XCenter-i,2)+pow(img1YCenter-j,2));
            dist2 = sqrt(pow(img2XCenter-i,2)+pow(img2YCenter-j,2));
            
            uint mask_result1_val = mask_result1.data[mask_result1.step[0]*i + mask_result1.step[1]*j];
            uint mask_result2_val = mask_result2.data[mask_result2.step[0]*i + mask_result2.step[1]*j];
            if((mask_result1_val + mask_result2_val) == 510)
            {
                if(dist1 < dist2)
                {
                    printf("IMG1 Closest to Centroid - Point(%d,%d) img1: %0.2f img2: %0.2f\n",i,j,dist1,dist2);
                    result.data[result.step[0]*i + result.step[1]* j + 0] = result1.data[result1.step[0]*i + result1.step[1]* j + 0];
                    result.data[result.step[0]*i + result.step[1]* j + 1] = result1.data[result1.step[0]*i + result1.step[1]* j + 1];
                    result.data[result.step[0]*i + result.step[1]* j + 2] = result1.data[result1.step[0]*i + result1.step[1]* j + 2];
                }
                else
                {
                    printf("IMG2 Closest to Centroid - Point(%d,%d) img1: %0.2f img2: %0.2f\n",i,j,dist1,dist2);
                    result.data[result.step[0]*i + result.step[1]* j + 0] = result2.data[result2.step[0]*i + result2.step[1]* j + 0];
                    result.data[result.step[0]*i + result.step[1]* j + 1] = result2.data[result2.step[0]*i + result2.step[1]* j + 1];
                    result.data[result.step[0]*i + result.step[1]* j + 2] = result2.data[result2.step[0]*i + result2.step[1]* j + 2];
                }
            }
            else
            {
                if(isNonZeroPixel(result1, i, j))
                {
                    result.data[result.step[0]*i + result.step[1]* j + 0] = result1.data[result1.step[0]*i + result1.step[1]* j + 0];
                    result.data[result.step[0]*i + result.step[1]* j + 1] = result1.data[result1.step[0]*i + result1.step[1]* j + 1];
                    result.data[result.step[0]*i + result.step[1]* j + 2] = result1.data[result1.step[0]*i + result1.step[1]* j + 2];
                }
                
                if(isNonZeroPixel(result2, i, j))
                {
                    result.data[result.step[0]*i + result.step[1]* j + 0] = result2.data[result2.step[0]*i + result2.step[1]* j + 0];
                    result.data[result.step[0]*i + result.step[1]* j + 1] = result2.data[result2.step[0]*i + result2.step[1]* j + 1];
                    result.data[result.step[0]*i + result.step[1]* j + 2] = result2.data[result2.step[0]*i + result2.step[1]* j + 2];
                }
                    
            }
        }
    }
}