//
//  TrackedObject.h
//  HW11
//
//  Created by Don Johnson on 4/11/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#ifndef __HW11__TrackedObject__
#define __HW11__TrackedObject__
#include "opencv2/video/tracking.hpp"

using namespace cv;

//Documentation for OpenCV's Kalman Filter
//http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html#kalmanfilter

//A class to help keep track of the history of tracked objects
class TrackedObject
{
public:
    TrackedObject(Point& firstMeasurement);
    Point getPrediction();
    void update(Point& measuredPoint);
    
    Point currentPosition;
    vector<Point> history;
    KalmanFilter estimator;
    Scalar color;
};

#endif /* defined(__HW11__TrackedObject__) */
