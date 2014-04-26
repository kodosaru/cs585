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
    
    // Create image of flood-fill regions
    Mat regions(binary.size(),CV_16UC1);
    if(DEBUG && regions.channels() != regions.channels())
        cout<< "Mark and binary matrices are different sizes"<<endl;
    
    // Confirm stack routines are working properly before running flood fill
    //testStack(binary,dataDir);
    //return 0;
    
    floodFill(binary, regions);
    
    // Display region map
    Mat tempRegions(regions.size(),CV_8UC1);
    regions.convertTo(tempRegions, CV_8UC1);
    
    double minVal;
    double maxVal;
    Point minLoc;
    Point maxLoc;
    minMaxLoc(tempRegions, &minVal, &maxVal, &minLoc, &maxLoc );
    adjustContrastBrightness(tempRegions, tempRegions, 0, 255.0f/maxVal);
    
    imshow("Regions",tempRegions);
    imwrite(dataDir+"regions.png",tempRegions);
    waitKey();
    
    return 0;
}
