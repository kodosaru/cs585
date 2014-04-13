//
//  Methods.h
//  HW11
//
//  Created by Don Johnson on 4/11/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#ifndef __HW11__Methods__
#define __HW11__Methods__
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/video/tracking.hpp"
#include <string>
#include <vector>
#include <iostream>
#include "TrackedObject.h"

#define MAX_IMAGES 4
#define MAX_OBJECTS 6

using namespace cv;
using namespace std;

//You have work to do in this function
//function to track the red object
void trackRedObjects(Mat& view, vector<TrackedObject>& tracks, vector<Point>& detections, vector<vector<Point>*> detectionsBuffer, int redThreshold, int areaThreshold);

//functions to detect the largest red object in an image
bool findRedObjects(Mat& view, vector<Point>& locations, vector<vector<Point> >& outlines, int redThreshold, int areaThreshold);

// Compute the area and center of a region bounded by an outline
void computeObjectAreaAndCenter(vector<Point>& outline, double& area, Point& center);

//Draw an outline stored in a vector of points
void drawLine(Mat& image, vector<Point>& outline, Scalar color);

//Compute distance between two points
double computeDistance(Point p1, Point p2);

//Original data association algorithm
void dataAssociationOriginal(vector<TrackedObject>& tracks, vector<Point>& detections);

//New data association algorithm which constrants bipartite graph and examines all paths
void dataAssociationNew(vector<TrackedObject>& tracks, vector<Point>& detections);

#endif /* defined(__HW11__Methods__) */
