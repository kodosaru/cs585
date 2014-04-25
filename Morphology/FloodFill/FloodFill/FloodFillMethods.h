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


inline void setPixel1C(PIXEL pixel, cv::Mat img)
{
    uchar *rowPtr=img.ptr<uchar>(pixel.pt.y);
    rowPtr[pixel.pt.x]=(uchar)pixel.val[0];
    if(DEBUG && pixel.val[3] != 0.0f)
        std::cout<<"Scalar[3] not equal to zero"<<std::endl;
}

inline void setPixel3C(PIXEL pixel, cv::Mat img)
{
    uchar *rowPtr=img.ptr<uchar>(pixel.pt.y);
    rowPtr[pixel.pt.x * 3]=(uchar)pixel.val[0];
    rowPtr[pixel.pt.x * 3 + 1]=(uchar)pixel.val[1];
    rowPtr[pixel.pt.x * 3 + 2]=(uchar)pixel.val[2];
    if(DEBUG && pixel.val[3] != 0.0f)
        std::cout<<"Scalar[3] not equal to zero"<<std::endl;
}

#endif /* defined(__FloodFill__FloodFillMethods__) */
