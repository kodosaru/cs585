//
//  Methods.cpp
//  KMeans
//
//  Created by Don Johnson on 4/20/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#include "Methods.h"

using namespace cv;
using namespace std;
namespace fs = boost::filesystem;

void createGraph3DGrayScale(Mat& graph, Mat& labels, int clusterCount)
{
    int i = 0;
    while (i < labels.rows)
    {
        int channels = graph.channels();
        if(channels != 1)
        {
            cout<<"Gray scale image must have one channel"<<endl;
            return;
        }
        for(int row=0; row<graph.rows; row++)
        {
            for(int col=0; col<graph.cols; col++)
            {
                int clusterIdx = labels.at<int>(i);
                graph.data[row*(graph.step[0]) + col] = clusterIdx;
                //printf("point(%d): (%u)\n",i,graph.data[row*(graph.step[0]) + col]);
                i++;
            }
        }
    }
}

void createGraph3D(Mat& graph, Mat& labels, int clusterCount)
{
    RNG rng(12345);
    vector<Scalar> colorTab;
    
    for(int i=0;i<clusterCount;i++)
    {
        colorTab.push_back(Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255)));
    }
    
    int i = 0;
    while (i < labels.rows)
    {
        int channels = graph.channels();
        if(channels != 3)
        {
            cout<<"Graph must have three channels"<<endl;
            return;
        }
        for(int row=0; row<graph.rows; row++)
        {
            for(int col=0; col<graph.cols; col++)
            {
                int clusterIdx = labels.at<int>(i);
                Scalar color = colorTab[clusterIdx];
                graph.data[row*(graph.step[0]) + col*channels + 0] = color[0];
                graph.data[row*(graph.step[0]) + col*channels + 1] = color[1];
                graph.data[row*(graph.step[0]) + col*channels + 2] = color[2];
                //printf("point(%d): (%u,%u,%u)\n",i,graph.data[row*(graph.step[0]) + col*channels + 0],graph.data[row*(graph.step[0]) + col*channels + 1],graph.data[row*(graph.step[0]) + col*channels + 2]);
                i++;
            }
        }
    }
}

void createGraph2D(Mat& graph, Mat& points, Mat& labels, Point2i dataRange, int clusterCount, unsigned long sampleCount)
{
    RNG rng(12345);
    graph = Scalar::all(0);
    scaleData2D(points, dataRange);
    vector<Scalar> colorTab;
    
    for(int i=0;i<clusterCount;i++)
    {
        colorTab.push_back(Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255)));
    }
    for(int i = 0; i < sampleCount; i++ )
    {
        // The cluster index runs from 0 to clusterCount - 1
        int clusterIdx = labels.at<int>(i);
        //cout<<"Cluster Index: "<<clusterIdx<<endl;
        Point ipt = points.at<Point2f>(i);
        circle(graph, ipt, 2, colorTab[clusterIdx], CV_FILLED, CV_AA );
    }
}

void scaleData(Mat& points, int dataRange)
{
    float xmin=FLT_MAX;
    float xmax=FLT_MIN;
    for(int i=0;i<points.rows;i++)
    {
        xmin = xmin < points.at<uchar>(i) ? xmin : points.at<uchar>(i);
        xmax = xmax > points.at<uchar>(i) ? xmax : points.at<uchar>(i);
    }
    printf("Data xmin: %0.2f xmax: %0.2f\n",xmin,xmax);
    float xscale=dataRange/(xmax-xmin);
    for(int i=0;i<points.rows;i++)
    {
        points.at<uchar>(i) = (uchar)((points.at<uchar>(i) - xmin) * xscale + 0.5);
    }
}

void scaleData2D(Mat& points, Point2i dataRange)
{
    float xmin=FLT_MAX;
    float xmax=FLT_MIN;
    float ymin=FLT_MAX;
    float ymax=FLT_MIN;
    for(int i=0;i<points.rows;i++)
    {
        xmin = xmin < points.at<Point2f>(i).x ? xmin : points.at<Point2f>(i).x;
        xmax = xmax > points.at<Point2f>(i).x ? xmax : points.at<Point2f>(i).x;
        ymin = ymin < points.at<Point2f>(i).y ? ymin : points.at<Point2f>(i).y;
        ymax = ymax > points.at<Point2f>(i).y ? ymax : points.at<Point2f>(i).y;
    }
    printf("Data xmin: %0.2f xmax: %0.2f ymin: %0.2f ymax: %0.2f\n",xmin,xmax,ymin,ymax);
    float xscale=dataRange.x/(xmax-xmin);
    float yscale=dataRange.y/(ymax-ymin);
    for(int i=0;i<points.rows;i++)
    {
        points.at<Point2f>(i).x = ((points.at<Point2f>(i).x - xmin)) * xscale;
        points.at<Point2f>(i).y = ((points.at<Point2f>(i).y - ymin)) * yscale;
    }
}

void scaleData3D(Mat& points, Point3i dataRange)
{
    float xmin=FLT_MAX;
    float xmax=FLT_MIN;
    float ymin=FLT_MAX;
    float ymax=FLT_MIN;
    float zmin=FLT_MAX;
    float zmax=FLT_MIN;
    for(int i=0;i<points.rows;i++)
    {
        xmin = xmin < points.at<Point3_<uchar>>(i).x ? xmin : points.at<Point3_<uchar>>(i).x;
        xmax = xmax > points.at<Point3_<uchar>>(i).x ? xmax : points.at<Point3_<uchar>>(i).x;
        ymin = ymin < points.at<Point3_<uchar>>(i).y ? ymin : points.at<Point3_<uchar>>(i).y;
        ymax = ymax > points.at<Point3_<uchar>>(i).y ? ymax : points.at<Point3_<uchar>>(i).y;
        zmin = zmin < points.at<Point3_<uchar>>(i).z ? zmin : points.at<Point3_<uchar>>(i).z;
        zmax = zmax > points.at<Point3_<uchar>>(i).z ? zmax : points.at<Point3_<uchar>>(i).z;
        
    }
    printf("Data xmin: %0.2f xmax: %0.2f ymin: %0.2f ymax: %0.2f zmin: %0.2f zmax: %0.2f\n",xmin,xmax,ymin,ymax,zmin,zmax);
    float xscale=dataRange.x/(xmax-xmin);
    float yscale=dataRange.y/(ymax-ymin);
    float zscale=dataRange.z/(zmax-zmin);
    for(int i=0;i<points.rows;i++)
    {
        points.at<Point3_<uchar>>(i).x = ((points.at<Point3_<uchar>>(i).x - xmin)) * xscale;
        points.at<Point3_<uchar>>(i).y = ((points.at<Point3_<uchar>>(i).y - ymin)) * yscale;
        points.at<Point3_<uchar>>(i).z = ((points.at<Point3_<uchar>>(i).z - zmin)) * zscale;
    }
}

void generateSampleData(Mat& graph, Mat& points, Mat& centers, int& clusterCount, unsigned long& sampleCount, int maxClusters)
{
    RNG rng(rand());
    
    clusterCount = rng.uniform(2, maxClusters+1);
    sampleCount = rng.uniform(1, 1001);
    points.create((int)sampleCount, 1, CV_32FC2);
    //printf("kmeans info  cluster count: %d sample count: %d points size: (%d,%d)\n", clusterCount, sampleCount, points.rows, points.cols);
    
    clusterCount = MIN(clusterCount, (int)sampleCount);
    centers.create(clusterCount, 1, points.type());
    
    /* generate random sample from multigaussian distribution */
    for(int k = 0; k < clusterCount; k++ )
    {
        Point center;
        center.x = rng.uniform(0, graph.cols);
        center.y = rng.uniform(0, graph.rows);
        Mat pointChunk = points.rowRange(k*(int)sampleCount/clusterCount,
                                         k == clusterCount - 1 ? (int)sampleCount :
                                         (k+1)*(int)sampleCount/clusterCount);
        rng.fill(pointChunk, CV_RAND_NORMAL, Scalar(center.x, center.y), Scalar(graph.cols*0.05, graph.rows*0.05));
    }
    
    randShuffle(points, 1, &rng);
    for(int i=0;i<points.rows;i++)
    {
        cout<<"points: ("<<points.at<Point2f>(i).x<<","<<points.at<Point2f>(i).y<<")"<<endl;
    }
    
}

void imageData(Mat& image, Mat& points, int& clusterCount, unsigned long& sampleCount, int maxClusters)
{
    clusterCount = maxClusters;
    sampleCount = image.total();
    points.create((int)sampleCount, 1, CV_32FC3);
    
    clusterCount = MIN(clusterCount, (int)sampleCount);
    printf("kmeans info  cluster count: %d sample count: %lu points size: (%d,%d)\n", clusterCount, sampleCount, points.rows, points.cols);
    Mat centers(clusterCount, 1, points.type());
    
    int i = 0;
    while (i < points.rows)
    {
        int channels = image.channels();
        if(channels != 3)
        {
            cout<<"Image must have three channels"<<endl;
            points.release();
            return;
        }
        for(int row=0; row<image.rows; row++)
        {
            for(int col=0; col<image.cols; col++)
            {
                points.at<Point3f>(i).x = (float)image.data[row*(image.step[0]) + col*channels + 0];
                points.at<Point3f>(i).y = (float)image.data[row*(image.step[0]) + col*channels + 1];
                points.at<Point3f>(i).z = (float)image.data[row*(image.step[0]) + col*channels + 2];
                //printf("point(%d): (%u,%u,%u)\n",i,image.data[row*(image.step[0]) + col*channels + 0],image.data[row*(image.step[0]) + col*channels + 1],image.data[row*(image.step[0]) + col*channels + 2]);
                //printf("point(%d): (%0.0f,%0.0f,%0.0f)\n",i,points.at<Point3f>(i).x,points.at<Point3f>(i).y,points.at<Point3f>(i).z);
                i++;
            }
        }
    }
}

