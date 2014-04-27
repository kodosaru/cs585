//
//  CountObjectsMethods.cpp
//  CountObjects
//
//  Created by Don Johnson on 4/26/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#include "CountObjectsMethods.h"

using namespace std;
using namespace cv;

void constructRegionBlobLists(vector<vector<PIXEL>*>& regionLists, vector<cv::vector<PIXEL>*>& blobLists)
{
    for(unsigned short i=0;i<USHRT_MAX;i++)
        regionLists[i]=new vector<PIXEL>;
    
    for(unsigned short i=0;i<USHRT_MAX;i++)
        blobLists[i]=new vector<PIXEL>;
}

void destroyRegionBlobLists(vector<vector<PIXEL>*>& regionLists, vector<vector<PIXEL>*>& blobLists)
{
    // Destory region and blob lists
    for(int i=0;i<USHRT_MAX;i++)
    {
        if(regionLists[i]!=nullptr)
        {
            regionLists[i]->erase(regionLists[i]->begin(),regionLists[i]->end());
            regionLists[i]->~vector();
        }
        if(blobLists[i]!=nullptr)
        {
            blobLists[i]->erase(blobLists[i]->begin(),blobLists[i]->end());
            blobLists[i]->~vector();
        }
    }
}

void extractblobs(Mat& regions, unsigned short& nRegion, vector<vector<PIXEL>*>& regionLists, unsigned short& nBlobs, vector<vector<PIXEL>*>& blobLists, string dataDir)
{
    // Display region map
    Mat tempRegions(regions.size(),CV_8UC1);
    regions.convertTo(tempRegions, CV_8UC1);
    
    PIXEL blobPix;
    nBlobs=0;
    
    cout<<"Before removing very small and large areas"<<endl;
    for(int i=0;i<nRegion;i++)
    {
        cout<<"Region "<<i+1<<" has "<<regionLists[i]->size()<<" pixels"<<endl;
    }
    
    for(int i=0;i<nRegion;i++)
    {
        if(regionLists[i]->size()<1000 || regionLists[i]->size()>30000)
        {
            for(int j=0;j<regionLists[i]->size();j++)
            {
                blobPix=(*regionLists[i])[j];
                //printf("Object pixel(%d,%d)=%0.0f\n",blobPix.pt.x,blobPix.pt.y,blobPix.val[0]);
                blobPix.val[0]=0;
                blobPix.val[1]=0;
                blobPix.val[2]=0;
                blobPix.val[2]=0;
                setPixel_8UC1(blobPix,tempRegions);
            }
            regionLists[i]->erase(regionLists[i]->begin(),regionLists[i]->end());
            regionLists[i]->~vector();
            regionLists[i]=nullptr;
        }
        else
        {
            nBlobs++;
        }
    }
    for(int i=0,j=0;i<nRegion;i++)
    {
        if(regionLists[i]!=nullptr)
        {
            for(vector<PIXEL>::iterator it = regionLists[i]->begin(); it != regionLists[i]->end(); ++it) {
                blobLists[j]->push_back(*it);
            }
            j++;
        }
    }
    
    cout<<endl<<"After removing very small and large areas"<<endl;
    float beta=50.0;
    int incVal=(int)((UCHAR_MAX-beta)/nBlobs + 0.5);
    for(int i=0;i<nBlobs;i++)
    {
        cout<<"Blob "<<i+1<<", originally region "<<(*blobLists[i])[0].val[0]<<", has "<<blobLists[i]->size()<<" pixels"<<endl;
        
        for(int j=0;j<blobLists[i]->size();j++)
        {
            blobPix=(*blobLists[i])[j];
            blobPix.val[0]=(i+1)*incVal+beta;
            blobPix.val[1]=0;
            blobPix.val[2]=0;
            blobPix.val[3]=0;
            setPixel_8UC1(blobPix,tempRegions);
        }
    }
    
    imshow("Regions",tempRegions);
    imwrite(dataDir+"regions.png",tempRegions);
    //waitKey();
}

