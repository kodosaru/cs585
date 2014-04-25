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

struct pixel
{
    cv::Point pt;
    cv::Scalar val;
};
typedef struct pixel PIXEL;
inline void setPixel1C(PIXEL pixel, cv::Mat img);
inline void setPixel3C(PIXEL pixel, cv::Mat img);

#endif /* defined(__FloodFill__FloodFillMethods__) */
