//
//  Functions.h
//  HW8
//
//  Created by Don Johnson on 3/26/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#ifndef __HW8__Functions__
#define __HW8__Functions__

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#define USE_MATH_DEFINES 1
#include <math.h>
#define N_SAVE_FRAMES 5
#define N_SKIP_FRAMES 1

using namespace cv;
using namespace std;

// copied from OpenCV sample fback.cpp
void opticalFlowMagnitudeAngle(const Mat& flow, Mat& magnitude, Mat& angle, int step);
void recordFrames(Mat& view, Mat& magnitude, Mat& angle, vector<Mat>& view_record, vector<Mat>& magnitude_record, vector<Mat>& angle_record, bool& bRecording, int frameNumber, int nFrame[], int& fcount);
void writeFrames(vector<Mat>& view_record, vector<Mat>& magnitude_record, vector<Mat>& angle_record, char* directory,int nFrame[]);
void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step, double, const Scalar& color);
void visualizeFlow(const Mat& magnitude, const Mat& angle, Mat& magnitude8UC1, Mat& angle8UC3);

#endif /* defined(__HW8__Functions__) */
