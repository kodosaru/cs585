//
//  File.h
//  Assignment6
//
//  Created by Don Johnson on 3/5/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#ifndef __Assignment6__Functions__
#define __Assignment6__Functions__

#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"

using namespace cv;
using namespace std;

Mat createMask(Mat& image);
#endif /* defined(__Assignment6__File__) */
