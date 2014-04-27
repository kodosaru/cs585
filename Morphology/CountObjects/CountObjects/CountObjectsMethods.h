//
//  CountObjectsMethods.h
//  CountObjects
//
//  Created by Don Johnson on 4/26/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#ifndef __CountObjects__CountObjectsMethods__
#define __CountObjects__CountObjectsMethods__

#include <iostream>
#include "opencv2/core/core.hpp"
#include "FloodFillMethods.h"

void constructRegionBlobLists(cv::vector<cv::vector<PIXEL>*>& regionLists, cv::vector<cv::vector<PIXEL>*>& blobLists);
void destroyRegionBlobLists(cv::vector<cv::vector<PIXEL>*>& regionLists, cv::vector<cv::vector<PIXEL>*>& blobLists);
void extractblobs(cv::Mat& regions, int clusterCount, unsigned short& nRegion, cv::vector<cv::vector<PIXEL>*>& regionLists, unsigned short& nBlobs, cv::vector<cv::vector<PIXEL>*>& blobLists,  std::string outputDataDir, std::string outputFileName);
int readInImage(cv::Mat& image, std::string inputDataDir, std::string fullInputfileName, std::string outputDataDir, std::string outputFileName, float resizeFactor);
unsigned long listFiles(std::string targetPath, cv::vector<std::string>& fileNames);
#endif /* defined(__CountObjects__CountObjectsMethods__) */
