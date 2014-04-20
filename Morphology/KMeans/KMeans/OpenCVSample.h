//
//  OpenCVSample.h
//  KMeans
//
//  Created by Don Johnson on 4/19/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#ifndef __KMeans__OpenCVSample__
#define __KMeans__OpenCVSample__

#include <iostream>
#include "OpenCVSample.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void generateSampleData(Mat& graph, Mat& points, Mat& centers, int& clusterCount, unsigned long& sampleCount, int maxClusters);
void createGraph3D(Mat& graph, Mat& labels, int clusterCount, int sampleCount);
void createGraph2D(Mat& graph, Mat& points, Mat& labels, Point2i dataRange, int clusterCount, int sampleCount);
void imageData(Mat& in, Mat& points, int& clusterCount, unsigned long& sampleCount, int maxClusters);
void scaleData2D(Mat& points, Point2i dataRange);
void scaleData3D(Mat& points, Point3i dataRange);

#endif /* defined(__KMeans__OpenCVSample__) */
