//
//  main.cpp
//  CountObjects
//
//  Created by Don Johnson on 4/26/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#include <iostream>
#include "FloodFillMethods.h"
#include "Stack.hpp"
#include "CountObjectsMethods.h"
#include "KMeansMethods.h"

using namespace std;
using namespace cv;

int main(int argc, const char * argv[])
{
    string dataDir="/Users/donj/workspace/cs585/Morphology/Data/Output/";
    
    // Read in program arguments
    if(argc!=4)
    {
        cout<<"Incorrect number of program arguments"<<endl;
        return 1;
    }
    string fileName=argv[1];
    
    bool bSaveState;
    if(strcmp(argv[2],"true")==0)
        bSaveState=true;
    else if(strcmp(argv[2],"false")==0)
        bSaveState=false;
    else
    {
        cout<<"You must specify whether to save the state of the k-means classification"<<endl;
    }
    int maxClusters=atoi(argv[3]);
    
    // Perform k-means classfication
    int clusterCount=0;
    kMeansCustom(bSaveState, fileName, maxClusters, clusterCount);

    // Set up flood fill segmentation
    char cn[256];
    sprintf(cn,"%s%s%s%d%s",dataDir.c_str(),argv[1],"Binary",clusterCount,".png");
    Mat binary=imread(cn,CV_LOAD_IMAGE_GRAYSCALE);
    imshow("Binary",binary);
    
    // Create data structures to save region and blob information
    unsigned short nRegions=0;
    vector<vector<PIXEL>*> regionLists(USHRT_MAX);
    unsigned short  nBlobs=0;
    vector<vector<PIXEL>*> blobLists(USHRT_MAX);
    constructRegionBlobLists(regionLists, blobLists);
    
    // Create image of flood-fill regions
    Mat regions(binary.size(),CV_16UC1);
    
    // Segment the binary object into regions
    floodFill(binary, regions, nRegions, regionLists);
    
    // Extract a list of object candiate blobs and the list of pixels in each
    extractblobs(regions, nRegions, regionLists, nBlobs, blobLists, dataDir);
    
    destroyRegionBlobLists(regionLists, blobLists);
    
    return 0;
}
