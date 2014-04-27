//
//  FloodFillMethods.h
//  FloodFill
//
//  Created by Don Johnson on 4/24/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#ifndef __FloodFill__FloodFillMethods__
#define __FloodFill__FloodFillMethods__

//opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
//C
#include <stdio.h>
//C++
#include <iostream>
#include <sstream>
#include <string>
#include <set>

//Mine
#include "Stack.hpp"

struct pixel
{
    cv::Point pt=cv::Point(-INT_MAX,-INT_MAX);
    cv::Scalar val=cv::Scalar(-FLT_MAX,-FLT_MAX,-FLT_MAX,-FLT_MAX);
};
typedef struct pixel PIXEL;
void testStack(cv::Mat& binary, cv::string dataDir);

void floodFill(cv::Mat& image, cv::Mat& regions, unsigned short& nRegions, cv::vector<cv::vector<PIXEL>*>& regionLists);
void adjustContrastBrightness(cv::Mat& image, cv::Mat& new_image, int beta, double alpha);

inline void setPixel_8UC1(PIXEL pixel, cv::Mat& image)
{
    // pixel -> image
    // Set the pixel in "image" at same (x,y) to same value as "pixel"
    if(image.type()!= CV_8UC1)
    {
        std::cout<<"Image must be a CV_8UC1 type";
        return;
    }
    if(!(pixel.val[1] == 0.0f && pixel.val[2] == 0.0f && pixel.val[3] == 0.0f))
        std::cout<<"Scalar[1,2 or 3] not equal to zero"<<std::endl;
    uchar *rowPtr=image.ptr<uchar>(pixel.pt.y);
    rowPtr[pixel.pt.x]=pixel.val[0];
}

inline void getPixel_8UC1(cv::Mat& image, PIXEL& pixel)
{
    // image -> pixel
    // get the pixel in "image" at same (x,y) and set "pixel" to same value
    if(image.type()!= CV_8UC1)
    {
        std::cout<<"Image must be a CV_8UC1 type";
        return;
    }
    uchar *rowPtr=image.ptr<uchar>(pixel.pt.y);
    pixel.val[0]=(float)rowPtr[pixel.pt.x];
    pixel.val[1]=0.0f;
    pixel.val[2]=0.0f;
    pixel.val[3]=0.0f;
}

inline void setPixel_16UC1(PIXEL pixel, cv::Mat& image)
{
    // pixel -> image
    // Set the pixel in "image" at same (x,y) to same value as "pixel"
    if(image.type()!= CV_16UC1)
    {
        std::cout<<"Image must be a CV_16UC1 type";
        return;
    }
    if(!(pixel.val[1] == 0.0f && pixel.val[2] == 0.0f && pixel.val[3] == 0.0f))
        std::cout<<"Scalar[1,2 or 3] not equal to zero"<<std::endl;
    unsigned short *rowPtr=image.ptr<unsigned short>(pixel.pt.y);
    rowPtr[pixel.pt.x]=pixel.val[0];
}

inline void getPixel_16UC1(cv::Mat& image, PIXEL& pixel)
{
    // image -> pixel
    // get the pixel in "image" at same (x,y) and set "pixel" to same value
    if(image.type()!= CV_16UC1)
    {
        std::cout<<"Image must be a CV_16UC1 type";
        return;
    }
    unsigned short *rowPtr=image.ptr<unsigned short>(pixel.pt.y);
    pixel.val[0]=(float)rowPtr[pixel.pt.x];
    pixel.val[1]=0.0f;
    pixel.val[2]=0.0f;
    pixel.val[3]=0.0f;
}

inline void setPixel_8UC3(PIXEL pixel, cv::Mat& image)
{
    // Set the pixel in "image" at same (x,y) to same value as "pixel"
    if(image.type()!= CV_8UC3)
    {
        std::cout<<"Image must be a CV_8UC3 type";
        return;
    }
    uchar *rowPtr=image.ptr<uchar>(pixel.pt.y);
    rowPtr[pixel.pt.x * 3]=pixel.val[0];
    rowPtr[pixel.pt.x * 3 + 1]=pixel.val[1];
    rowPtr[pixel.pt.x * 3 + 2]=pixel.val[2];
    if(pixel.val[3] != 0.0f)
        std::cout<<"Scalar[3] not equal to zero"<<std::endl;
}

inline void getPixel_8UC3(cv::Mat& image, PIXEL& pixel)
{
    // image -> pixel
    // get the pixel in "image" at same (x,y) and set "pixel" to same value
    if(image.type()!= CV_8UC3)
    {
        std::cout<<"Image must be a CV_8UC3 type";
        return;
    }
    uchar *rowPtr=image.ptr<uchar>(pixel.pt.y);
    pixel.val[0]=(float)rowPtr[pixel.pt.x * 3];
    pixel.val[1]=(float)rowPtr[pixel.pt.x * 3 + 1];
    pixel.val[2]=(float)rowPtr[pixel.pt.x * 3 + 2];
    pixel.val[3]=0.0f;
}

inline bool pixelInBounds(cv::Mat& img, cv::Point pt)
{
    if(pt.x<0 || pt.x>= img.cols)
        return false;
    else if(pt.y<0 || pt.y>= img.rows)
        return false;
    else
        return true;
}

#endif /* defined(__FloodFill__FloodFillMethods__) */
