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
    
    // Confirm stack routines are working properly before running flood fill
    //testStack(binary,dataDir);
    //return 0;
    
    vector<vector<PIXEL>*> regionsList(USHRT_MAX);
    for(unsigned short i=0;i<USHRT_MAX;i++)
        regionsList[i]=new vector<PIXEL>;
    unsigned short nRegion;
    floodFill(binary, regions, nRegion, regionsList);

    // Display region map
    Mat tempRegions(regions.size(),CV_8UC1);
    regions.convertTo(tempRegions, CV_8UC1);
    
    double minVal;
    double maxVal;
    Point minLoc;
    Point maxLoc;
    minMaxLoc(tempRegions, &minVal, &maxVal, &minLoc, &maxLoc );

    cvtColor(tempRegions, tempRegions, CV_GRAY2BGR);
    PIXEL objectPix;
    int nObjects=0;
    
    cout<<"Before removing very small and large areas"<<endl;
    for(int i=0;i<nRegion;i++)
    {
        cout<<"Region "<<i+1<<" has "<<regionsList[i]->size()<<" pixels"<<endl;
    }

    for(int i=0;i<nRegion;i++)
    {
        if(regionsList[i]->size()<1000 || regionsList[i]->size()>30000)
        {
            for(int j=0;j<regionsList[i]->size();j++)
            {
                objectPix=(*regionsList[i])[j];
                //printf("Object pixel(%d,%d)=%0.0f\n",objectPix.pt.x,objectPix.pt.y,objectPix.val[0]);
                objectPix.val[0]=0;
                objectPix.val[1]=0;
                objectPix.val[2]=255;
                setPixel_8UC3(objectPix,tempRegions);
            }
            regionsList[i]->erase(regionsList[i]->begin(),regionsList[i]->end());
            regionsList[i]->~vector();
            regionsList[i]=nullptr;
         }
        else
        {
            nObjects++;
        }
    }
    
    vector<vector<PIXEL>*> objectsList(nObjects);
    for(int i=0,j=0;i<nRegion;i++)
    {
        if(regionsList[i]!=nullptr)
        {
            objectsList[j++]=regionsList[i];
            regionsList[i]=nullptr;
        }
    }
    
    cout<<endl<<"After removing very small and large areas"<<endl;
    for(int i=0;i<nObjects;i++)
    {
            cout<<"Object "<<i+1<<" originally region "<<(*objectsList[i])[0].val[0]<<" has "<<objectsList[i]->size()<<" pixels"<<endl;
    }
    
    for(int i=0;i<nObjects;i++)
    {
        if(objectsList[i]!=nullptr)
        {
            objectsList[i]->erase(objectsList[i]->begin(),objectsList[i]->end());
            objectsList[i]->~vector();
        }
    }
    
    adjustContrastBrightness(tempRegions, tempRegions, 50, 255.0f/maxVal);
    imshow("Regions",tempRegions);
    imwrite(dataDir+"regions.png",tempRegions);
    waitKey();
    
    return 0;
}
