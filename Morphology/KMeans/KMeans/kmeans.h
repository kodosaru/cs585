//
//  OpenCVSample.h
//  KMeans
//
//  Created by Don Johnson on 4/19/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#ifndef __KMeans__kmeans__
#define __KMeans__kmeans__

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "stdlib.h"

void createGraph3DGrayScale(cv::Mat& graph, cv::Mat& labels, int clusterCount);
void createGraph3D(cv::Mat& graph, cv::Mat& labels, int clusterCount);
void createGraph2D(cv::Mat& graph, cv::Mat& points, cv::Mat& labels, cv::Point2i dataRange, int clusterCount, unsigned long sampleCount);
void generateSampleData(cv::Mat& graph, cv::Mat& points, cv::Mat& centers, int& clusterCount, unsigned long& sampleCount, int maxClusters);
void imageData(cv::Mat& in, cv::Mat& points, int& clusterCount, unsigned long& sampleCount, int maxClusters);
void scaleData(cv::Mat& points, int dataRange);
void scaleData2D(cv::Mat& points, cv::Point2i dataRange);
void scaleData3D(cv::Mat& points, cv::Point3i dataRange);

#endif /* defined(__KMeans__kmeans__) */
