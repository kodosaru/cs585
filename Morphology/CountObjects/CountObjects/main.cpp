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
#include "Moments.h"

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
    
    // Calculate centroids of blobs
    Mat tempRegions=imread(dataDir+"regions.png",CV_8UC1);
    for(int i=0;i<nBlobs;i++)
    {
        if(blobLists[i]!=nullptr)
        {
            cout<<"Centroid of blob["<<i<<"]: ("<<xbar(*blobLists[i])<<","<<ybar(*blobLists[i])<<")"<<endl;
            cout<<Mij(*blobLists[i], 1, 1) - xbar(*blobLists[i]) * Mij(*blobLists[i], 0, 1)<<endl;
            //Mij(v, 1, 1) - xbar(v) * Mij(v, 0, 1)
            cout<<muij((*blobLists[i]), 1, 1)<<endl;
            cout<<Mij(*blobLists[i], 1, 1) - ybar(*blobLists[i]) * Mij(*blobLists[i], 1, 0)<<endl;
        }
        else
        {
            cout<<"Blob "<<i<<" has a null pointer"<<endl;
        }
    }
    imshow("Regions",tempRegions);
    waitKey();
    
    destroyRegionBlobLists(regionLists, blobLists);
    
    return 0;
}
