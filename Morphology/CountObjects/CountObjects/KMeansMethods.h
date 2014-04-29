//
//  Methods.h
//  KMeans
//
//  Created by Don Johnson on 4/20/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#ifndef __KMeans__Methods__
#define __KMeans__Methods__

//opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
//C
#include <stdio.h>
//C++
#include <iostream>
#include <sstream>
#include <string>
#include <set>

struct mouseInfo
{
    std::vector<cv::Point2i> points;
    cv::Mat labels;
    cv::Mat graph;
    std::set<int> completedClasses;
};
typedef struct mouseInfo MOUSEINFO;

int findClosestCenter(cv::Mat& centers, cv::Scalar imagePixel);
int useSavedCentersAndClasses(bool bSaveState, std::string dataDir, std::string fileName, int maxClusters, int& clusterCount);
static void onMouse( int event, int x, int y, int /*flags*/, void* /*param*/ );
int kMeansCustom(bool bSaveState, std::string dataDir, std::string fileName, int maxClusters, int& clusterCount);
void saveCompletedClasses(std::set<int>& completedClasses, std::string path);
void loadCompletedClasses(std::set<int>& completedClasses, std::string path);
void colorTabTest(int clusterCount, std::string dataDir);
void createGraph3DGrayScale(cv::Mat& graph, cv::Mat& labels, int clusterCount);
void createGraph3D(cv::Mat& graph, cv::Mat& labels, int clusterCount, std::string dataDir, bool bSaveState);
void createGraph2D(cv::Mat& graph, cv::Mat& points, cv::Mat& labels, cv::Point2i dataRange, int clusterCount, unsigned long sampleCount);
void scaleData(cv::Mat& points, int dataRange);
void scaleData2D(cv::Mat& points, cv::Point2i dataRange);
void scaleData3D(cv::Mat& points, cv::Point3i dataRange);
void generateSampleData(cv::Mat& graph, cv::Mat& points, cv::Mat& centers, int& clusterCount, unsigned long& sampleCount, int maxClusters);
void imageData(cv::Mat& image, cv::Mat& points, int& clusterCount, unsigned long& sampleCount, int maxClusters);

#endif /* defined(__KMeans__Methods__) */
