//
//  main.cpp
//  FloodFill
//
//  Created by Don Johnson on 4/24/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//
#define DEBUG 1

#include <iostream>
#include "FloodFillMethods.h"
#include "Stack.hpp"
#include "CountObjectsMethods.h"

using namespace std;
using namespace cv;

int main(int argc, const char * argv[])
{
    string dataDir="/Users/donj/workspace/cs585/Morphology/Data/Output/";
    int clusterCount=20;
    char cn[256];
    sprintf(cn,"%s%s%s%d%s",dataDir.c_str(),argv[1],"Binary",clusterCount,".png");
    Mat binary=imread(cn,CV_LOAD_IMAGE_GRAYSCALE);
    imshow("Binary",binary);
    unsigned short nRegions=0;
    vector<vector<PIXEL>*> regionLists(USHRT_MAX);
    unsigned short  nBlobs=0;
    vector<vector<PIXEL>*> blobLists(USHRT_MAX);
    for(unsigned short i=0;i<USHRT_MAX;i++)
        regionLists[i]=new vector<PIXEL>;
    
    for(unsigned short i=0;i<USHRT_MAX;i++)
        blobLists[i]=new vector<PIXEL>;

        // Create image of flood-fill regions
    Mat regions(binary.size(),CV_16UC1);
    
    // Segment the binary object into regions
    floodFill(binary, regions, nRegions, regionLists);
    
    return 0;
}
