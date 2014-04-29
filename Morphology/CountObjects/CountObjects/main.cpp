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
#include <sstream>

#define FILLED -1

using namespace std;
using namespace cv;

double euclidDist(vector<double> u, vector<double> v)
{
    if(u.size()!=v.size())
    {
        cout<<"Unable to compute Euclidean distance - vectors are of a different length";
        return -DBL_MAX;
    }
    double sum=0.0;
    for(int i=0;i<u.size();i++)
        sum+=pow(u[i] - v[i], 2);
    sum=sqrt(sum);
    return sum;
}

int main(int argc, const char * argv[])
{
    // Input arguments
    // 0 program name
    // 1 full input file name with extension
    // 2 output file name without extension used to save intermiediate products
    // 3 boolen whether to save k-means state: background classes and class palette
    // 4 maximum number k-means classes to allow
    
    string inputDataDir="/Users/donj/workspace/cs585/Morphology/Data/Input/";
    string outputDataDir="/Users/donj/workspace/cs585/Morphology/Data/Output/";
    
    if(argc!=5)
    {
        cout<<"Incorrect number of program arguments"<<endl;
        return 1;
    }
    
    // Read in program arguments
    string inputFullFileName=argv[1];
    string outputFileName=argv[2];
    Mat input;
    if(readInImage(input, inputDataDir, inputFullFileName, outputDataDir, outputFileName, 1.0/3.0))
    {
        cout<<"Unable to read input file "<<inputDataDir+inputFullFileName<<endl;
        return 1;        
    }
    bool bSaveState;
    if(strcmp(argv[3],"true")==0)
        bSaveState=true;
    else if(strcmp(argv[3],"false")==0)
        bSaveState=false;
    else
    {
        cout<<"You must specify whether to save the state of the k-means classification"<<endl;
    }
    int maxClusters=atoi(argv[4]);
    
    // Perform k-means classfication
    int clusterCount=0;
    if(bSaveState)
    {
        if(kMeansCustom(bSaveState, outputDataDir, outputFileName, maxClusters, clusterCount))
            return 1;
    }
    else
    {
        if(useSavedCentersAndClasses(bSaveState, outputDataDir, outputFileName, maxClusters, maxClusters))
            return 1;
    }

    // Set up flood fill segmentation
    char cn[256];
    sprintf(cn,"%s%s%s%d%s",outputDataDir.c_str(),outputFileName.c_str(),"Binary",clusterCount,".png");
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
    extractblobs(regions, clusterCount, nRegions, regionLists, nBlobs, blobLists, outputDataDir, outputFileName);
    
    // Calculate centroids of blobs
    sprintf(cn,"%s%s%s%d%s",outputDataDir.c_str(),outputFileName.c_str(),"Regions",clusterCount,".png");
    Mat tempRegions=imread(cn,CV_8UC1);
    for(int i=0;i<nBlobs;i++)
    {
        if(blobLists[i]!=nullptr)
        {
            // Test code for moments
            //cout<<Mij(*blobLists[i], 1, 1) - xbar(*blobLists[i]) * Mij(*blobLists[i], 0, 1)<<endl;
            //Mij(v, 1, 1) - xbar(v) * Mij(v, 0, 1)
            //cout<<muij((*blobLists[i]), 1, 1)<<endl;
            //cout<<Mij(*blobLists[i], 1, 1) - ybar(*blobLists[i]) * Mij(*blobLists[i], 1, 0)<<endl;

            Scalar labelColor;
            Point centroid(xbar(*blobLists[i]), ybar(*blobLists[i]));
            stringstream ss;
            ss << i;
            string sVal = ss.str();
            vector<PIXEL> blob=*blobLists[i];
            if (blob[0].val[0]<=128)
                labelColor=CV_RGB(180,180,180);
            else
                labelColor=CV_RGB(80,80,80);
            circle(tempRegions, centroid, 5, labelColor, FILLED);
            putText(tempRegions, sVal, Point(centroid.x+4,centroid.y-1),FONT_HERSHEY_SIMPLEX, 0.45, labelColor,1);
            
            // Begin blob statistics and moments
            cout<<endl<<"Blob["<<i<<"]"<<endl;
            cout<<"Centroid of blob: ("<<centroid.x<<","<<centroid.y<<")"<<endl;
            
            /*
            Mat *covar = covarianceMatrix(*blobLists[i]);
            cout<<"Covariance Matrix"<<endl;
            cout<<"| "<<covar->at<double>(0,0)<<" "<<covar->at<double>(0,1)<<"|"<<endl;
            cout<<"| "<<covar->at<double>(1,0)<<" "<<covar->at<double>(1,1)<<"|"<<endl;
            Mat eval, evec;
            eigen((*covar),eval,evec);
            // Printing out column order and taking negative of eigenvectors like MATLAB
            evec = -1.0 * evec;
            cout<<"Eigenvector Matrix"<<endl;
            cout<<"| "<<evec.at<double>(1,0)<<" "<<evec.at<double>(0,0)<<"|"<<endl;
            cout<<"| "<<evec.at<double>(1,1)<<" "<<evec.at<double>(0,1)<<"|"<<endl;
            cout<<"OpenCV Calc Eigenvalue Matrix"<<endl;
            cout<<"| "<<eval.at<double>(1,0)<<" "<<0<<"|"<<endl;
            cout<<"| "<<0<<" "<<eval.at<double>(0,0)<<"|"<<endl;
            evec = *eigenvalueMatrix(*blobLists[i]);
            cout<<"Moment Calc Eigenvalue Matrix"<<endl;
            cout<<"| "<<eval.at<double>(1,0)<<" "<<0<<"|"<<endl;
            cout<<"| "<<0<<" "<<eval.at<double>(0,0)<<"|"<<endl;
            cout<<"Eccentricity: "<<eccentricity(*blobLists[i])<<endl;
            covar->release();
            */

            // Draw major axis
            Mat *orient = orientation(*blobLists[i]);
            double angle = orient->at<double>(0,0);
            cout<<"Orientation: "<<angle*180/M_PI<<" deg"<<endl;
            Scalar lineColor = CV_RGB(0,0,0);
            int d = 40;
            int deltaX = int(d * tan(angle) + 0.5);
            circle(tempRegions, Point(centroid.x+deltaX,centroid.y-d), 2, lineColor, FILLED);
            circle(tempRegions, Point(centroid.x-deltaX,centroid.y+d), 2, lineColor, FILLED);
            line(tempRegions, Point(centroid.x+deltaX,centroid.y-d), Point(centroid.x-deltaX,centroid.y+d), lineColor);
            orient->release();
            
            /*
            printf("Raw Moments: %0.2f  %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f  %0.2f %0.2f %0.2f\n",Mij(*blobLists[i],0,0), Mij(*blobLists[i],1,0),  Mij(*blobLists[i],0,1),  Mij(*blobLists[i],2,0),  Mij(*blobLists[i],1,1),  Mij(*blobLists[i],0,2), Mij(*blobLists[i],3,0),  Mij(*blobLists[i],2,1), Mij(*blobLists[i],1,2),  Mij(*blobLists[i],0,3));
            printf("Central Moments: %0.2f  %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f\n",muij(*blobLists[i],2,0), muij(*blobLists[i],1,1), muij(*blobLists[i],0,2),muij(*blobLists[i],3,0), muij(*blobLists[i],2,1), muij(*blobLists[i],1,2), muij(*blobLists[i],0,3));

            printf("Normalized Moments: %0.2f  %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f\n",etaij(*blobLists[i],2,0), etaij(*blobLists[i],1,1), etaij(*blobLists[i],0,2),etaij(*blobLists[i],3,0), etaij(*blobLists[i],2,1), etaij(*blobLists[i],1,2), etaij(*blobLists[i],0,3));
            */

            // Hu moments
            vector<vector<double>> huMoments(nBlobs);
             for(int j=1;j<=8;j++)
            {
                huMoments[i].push_back(Hui(*blobLists[i],j));
            }
            cout<<"Hu Invarients:";
            for(int j=0;j<7;j++)
                printf(" %0.2f",huMoments[i][j]);
            cout<<endl;
        }
        else
        {
            cout<<"Blob "<<i<<" has a null pointer"<<endl;
        }
        
        cout<<"Euclidean distance between object descriptions"<<endl;
        for(int i=0;i<nBlobs;i++)
            printf("\t%d",i);
        cout<<endl;
        double dist = 0.0;
        /*for(int i=0;i<nBlobs;i++)
            for(int j=0;j<nBlobs;j++)
            {
                if(i<j)
                    euclidDist(huMoments[i], vector<double> v);
            }*/
 

    }
    
    destroyRegionBlobLists(regionLists, blobLists);

    sprintf(cn,"%s%s%s%d%s",outputDataDir.c_str(),outputFileName.c_str(),"Blobs",clusterCount,".png");
    imwrite(cn,tempRegions);
    imshow("Blobs",tempRegions);
    waitKey();
    
    return 0;
}
