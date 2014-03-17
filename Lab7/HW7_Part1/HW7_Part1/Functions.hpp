//
//  Functions.h
//  HW7_Part1
//
//  Created by Don Johnson on 3/16/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#ifndef __HW7_Part1__Functions__
#define __HW7_Part1__Functions__

#include <iostream>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctype.h>

using namespace cv;
using namespace std;

void trackTemplate(Point2f& point, Mat& patch, Mat& image, Mat& displayGray, int pointNo, int searchRadius);
void matchingMethod(Mat& img, Mat& templ, Point& maxLoc, int pointNo, int match_method);

#endif /* defined(__HW7_Part1__Functions__) */
