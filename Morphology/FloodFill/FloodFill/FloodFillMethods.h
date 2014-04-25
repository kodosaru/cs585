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
    cv::Point pt;
    cv::Scalar val;
};
typedef struct pixel PIXEL;
void testStack(cv::Mat& binary, cv::string dataDir);

inline void setPixel1C(cv::Mat img, PIXEL pixel)
{
    uchar *rowPtr=img.ptr<uchar>(pixel.pt.y);
    rowPtr[pixel.pt.x]=(uchar)pixel.val[0];
    if(DEBUG && pixel.val[3] != 0.0f)
        std::cout<<"Scalar[3] not equal to zero"<<std::endl;
}

inline void setPixel3C(cv::Mat img, PIXEL pixel)
{
    uchar *rowPtr=img.ptr<uchar>(pixel.pt.y);
    rowPtr[pixel.pt.x * 3]=(uchar)pixel.val[0];
    rowPtr[pixel.pt.x * 3 + 1]=(uchar)pixel.val[1];
    rowPtr[pixel.pt.x * 3 + 2]=(uchar)pixel.val[2];
    if(DEBUG && pixel.val[3] != 0.0f)
        std::cout<<"Scalar[3] not equal to zero"<<std::endl;
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

inline bool neighborsInBound(cv::Mat& img, cv::Point pt, int nWay)
{
    cv::Point tpt=pt;
    if(nWay==4 || nWay==8)
    {
        tpt=cv::Point(pt.x-1,pt.y);
        if(!pixelInBounds(img,tpt))
            return false;
        tpt=cv::Point(pt.x+1,pt.y);
        if(!pixelInBounds(img,tpt))
            return false;
        tpt=cv::Point(pt.x,pt.y-1);
        if(!pixelInBounds(img,tpt))
            return false;
        tpt=cv::Point(pt.x,pt.y-1);
        if(!pixelInBounds(img,tpt))
            return false;
    }
    if(nWay==8)
    {
        tpt=cv::Point(pt.x-1,pt.y+1);
        if(!pixelInBounds(img,tpt))
            return false;
        tpt=cv::Point(pt.x+1,pt.y+1);
        if(!pixelInBounds(img,tpt))
            return false;
        tpt=cv::Point(pt.x-1,pt.y-1);
        if(!pixelInBounds(img,tpt))
            return false;
        tpt=cv::Point(pt.x+1,pt.y-1);
        if(!pixelInBounds(img,tpt))
            return false;
    }
    // If pixel passes all of the tests, its in bounds
    return true;
}

#endif /* defined(__FloodFill__FloodFillMethods__) */
