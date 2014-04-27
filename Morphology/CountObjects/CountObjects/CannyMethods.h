//
//  Methods.h
//  CountObjects
//
//  Created by Don Johnson on 4/8/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#ifndef __Canny__Methods__
#define __Canny__Methods__

#include <iostream>

using namespace cv;
using namespace std;

void callbackCanny(int, void*);
void CannyThreshold(Mat& src, Mat& dst, int lowThreshold, int highThreshold, int kernelSize);

#endif /* defined(__CountObjects__Methods__) */
