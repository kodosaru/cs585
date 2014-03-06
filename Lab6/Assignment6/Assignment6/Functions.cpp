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
