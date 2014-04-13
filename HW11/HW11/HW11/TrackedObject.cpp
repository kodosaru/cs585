//
//  TrackedObject.cpp
//  HW11
//
//  Created by Don Johnson on 4/11/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//
#include "TrackedObject.h"

using namespace cv;
using namespace std;


//Documentation for OpenCV's Kalman Filter
//http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html#kalmanfilter


TrackedObject::TrackedObject(Point& firstMeasurement):estimator(4,2,0)
{
    // Required: Write down the matrix that is being constructed here and explain what it means
    estimator.transitionMatrix.at<float>(0,0) = 1;
    estimator.transitionMatrix.at<float>(0,2) = 1;
    estimator.transitionMatrix.at<float>(1,1) = 1;
    estimator.transitionMatrix.at<float>(1,3) = 1;
    estimator.transitionMatrix.at<float>(2,2) = 1;
    estimator.transitionMatrix.at<float>(3,3) = 1;
    
    setIdentity(estimator.measurementMatrix);
    setIdentity(estimator.processNoiseCov);
    setIdentity(estimator.measurementNoiseCov);
    setIdentity(estimator.errorCovPost);
    
    estimator.statePost.at<float>(0) = firstMeasurement.x;
    estimator.statePost.at<float>(1) = firstMeasurement.y;
    
    currentPosition = firstMeasurement;
}

Point TrackedObject::getPrediction()
{
    Point predictedPoint;
    Mat prediction = estimator.predict();
    predictedPoint.x = prediction.at<float>(0);
    predictedPoint.y = prediction.at<float>(1);
    return predictedPoint;
}

void TrackedObject::update(Point& measuredPoint)
{
    Mat measurement = Mat::zeros(2, 1, CV_32F);
    measurement.at<float>(0) = measuredPoint.x;
    measurement.at<float>(1) = measuredPoint.y;
    estimator.correct(measurement);
    
    currentPosition.x = estimator.statePost.at<float>(0);
    currentPosition.y = estimator.statePost.at<float>(1);
    
    //appends a copy of the currentPosition, not a reference / pointer to it
    history.push_back(currentPosition);
}

