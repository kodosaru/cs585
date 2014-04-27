 //
//  Methods.cpp
//  KMeans
//
//  Created by Don Johnson on 4/20/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#include "KMeansMethods.h"
#include "Archive.hpp"
//Boost
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"

using namespace cv;
using namespace std;
namespace fs = boost::filesystem;

void kMeansCustom(bool bSaveState, std::string dataDir, std::string fileName, int maxClusters, int& clusterCount)
{
    MOUSEINFO mouseInfo,*pMouseInfo;
    pMouseInfo=&mouseInfo;
    Mat points,centers,grayScale,mask;
    clusterCount = 0;
    unsigned long sampleCount = 0;

    // Generate image data
    Mat image = imread(dataDir+fileName+".png");
    imshow("Input to KMeans",image);
    //waitKey();
    int ucharMax=pow(2,sizeof(uchar)*8) - 1;
    Point3i dataRange3D(ucharMax,ucharMax,ucharMax);
    cout<<"Fill points array"<<endl;
    imageData(image, points, clusterCount, sampleCount, maxClusters);
    
    // Perform k-means clustering
    cout<<"Perform k-means clustering"<<endl;
    ostringstream ss;
    ss<<clusterCount;
    
    // User wants to define new background classes
    if(bSaveState)
    {
        TermCriteria termCrit=TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0);
        kmeans(points, clusterCount, pMouseInfo->labels, termCrit, 3, KMEANS_PP_CENTERS, centers);
        saveMat(centers, dataDir+"centers."+ss.str()+".bin");
    }
    // User wants to use already defined background classes
    else
    {
        loadMat(centers, dataDir+"centers."+ss.str()+".bin");
        for(int i=0;i<centers.rows;i++)
            printf("Before Center(%d): (%0.2f,%0.2f,%0.2f)\n",i,centers.at<float>(i,0),centers.at<float>(i,1),centers.at<float>(i,3));
        TermCriteria termCrit=TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0);
        kmeans(points, clusterCount, pMouseInfo->labels, termCrit, 3, KMEANS_PP_CENTERS, centers);
    }
    for(int i=0;i<centers.rows;i++)
        printf("After Center(%d): (%0.2f,%0.2f,%0.2f)\n",i,centers.at<float>(i,0),centers.at<float>(i,1),centers.at<float>(i,3));
    
    // Display data clusters
    cout<<"Create graph cluster"<<endl;
    pMouseInfo->graph.create(image.rows,image.cols,CV_8UC3);
    createGraph3D(pMouseInfo->graph, pMouseInfo->labels, clusterCount, dataDir, bSaveState);
    char cn[256];
    sprintf(cn,"%s%s%s%d%s",dataDir.c_str(),fileName.c_str(),"clusters",clusterCount,".png");
    imwrite(cn,pMouseInfo->graph);
    imshow("Clusters",pMouseInfo->graph);
    //waitKey();
    
   if(bSaveState)
    {
        // Show graph and start defining background
        setMouseCallback("Clusters", onMouse, (void*) pMouseInfo);
        
        // Wait until the user is done defining the background classses
        int retKey;
        while(true)
        {
            retKey = waitKey(33);
            if( retKey == 'q' || retKey == 'Q' || retKey == 27)
                break;
        }

        saveCompletedClasses(pMouseInfo->completedClasses, dataDir+"completedClasses."+ss.str()+".bin");
    }
    
    // Reload graph (clusters image) to erase red circles and remove completed background classes
    pMouseInfo->graph=imread(cn);
    loadCompletedClasses(pMouseInfo->completedClasses, dataDir+"completedClasses."+ss.str()+".bin");
    int channels=pMouseInfo->graph.channels();
    for(int i=0;i<pMouseInfo->labels.rows;i++)
    {
        for(set<int>::iterator it=pMouseInfo->completedClasses.begin();it!=pMouseInfo->completedClasses.end();)
        {
            if(pMouseInfo->labels.at<int>(i)==*it)
            {
                pMouseInfo->graph.data[i*channels] = 0;
                pMouseInfo->graph.data[i*channels + 1] = 0;
                pMouseInfo->graph.data[i*channels + 2] = 0;
            }
            ++it;
        }
    }
    
    // Show final result after background removed
    printf("K-means info  cluster count: %d sample count: %lu points size: (%d,%d) labels size: (%d,%d) centers size: (%d,%d)\n", clusterCount, sampleCount, points.rows, points.cols, pMouseInfo->labels.rows, pMouseInfo->labels.cols, centers.rows, centers.cols);
    sprintf(cn,"%s%s%s%d%s",dataDir.c_str(),fileName.c_str(),"clustersMinusBackground",clusterCount,".png");
    imwrite(cn,pMouseInfo->graph);
    imshow("Desired Clusters", pMouseInfo->graph);
    waitKey();
    
    // Convert to grayscale
    mask.create(image.rows,image.cols,CV_8UC1);
    cvtColor(pMouseInfo->graph, mask, CV_BGR2GRAY);
    imshow("Gray Scale",mask);
    //waitKey();
    
    // Do opening to get rid of speckles
    erode(mask,mask,Mat());
    Mat dilateElement(5,5,CV_8UC1);
    dilateElement=Scalar(255,255,255);
    dilate(mask,mask,Mat());
    imshow("Gray Scale",mask);
    //waitKey();
    
    // Do binary threshold
    threshold(mask, mask, 0, 255, THRESH_BINARY);
    imshow("Mask", mask);
    //waitKey();
    
    // Convert float element to uchar
    sprintf(cn,"%s%s%s%d%s",dataDir.c_str(),fileName.c_str(),"Binary",clusterCount,".png");
    imwrite(cn,mask);
}

static void onMouse( int event, int x, int y, int /*flags*/, void* ptr )
{
    if( event == CV_EVENT_LBUTTONDOWN )
    {
        MOUSEINFO *pMouseInfo=(MOUSEINFO*)ptr;
        if(pMouseInfo==NULL||pMouseInfo->labels.total()==0||pMouseInfo->graph.total()==0)
        {
            cout<<"pMouseInfor is NULL or size of labels is 0 or size for graph is 0"<<endl;
            return;
        }
        pMouseInfo->points.push_back(Point(x, y));
        cout << "Point ("<<x<<","<<y<<") of "<<pMouseInfo->points.size()<<" total points"<<endl;
        circle(pMouseInfo->graph, Point(x,y), 5, CV_RGB(255,0,0), -1);
        int i = y * pMouseInfo->graph.cols + x;
        if(i > INT_MAX)
            cout<<"Integer maximum exceeded"<<endl;
        if(i > pMouseInfo->labels.rows)
            cout<<"Length of labels table exceeded"<<endl;
        int cls = pMouseInfo->labels.at<int>(i);
        printf("Class at (%d,%d) offset %d has value %d\n",x,y,i,cls);
        if(pMouseInfo->completedClasses.find(cls) == pMouseInfo->completedClasses.end())
        {
            cout<<"Inserting class "<<cls<<" into completed classes set"<<endl;
            pMouseInfo->completedClasses.insert(cls);
            int channels=pMouseInfo->graph.channels();
            for(int i=0;i<pMouseInfo->labels.rows;i++)
                if(pMouseInfo->labels.at<int>(i)==cls)
                {
                    pMouseInfo->graph.data[i*channels] = 0;
                    pMouseInfo->graph.data[i*channels + 1] = 0;
                    pMouseInfo->graph.data[i*channels + 2] = 0;
                }
        }
        for(int i;i<pMouseInfo->points.size();i++)
            circle(pMouseInfo->graph, pMouseInfo->points[i], 5, CV_RGB(255,0,0), -1);
        imshow("Clusters", pMouseInfo->graph);
    }
}

void saveCompletedClasses(set<int>& completedClasses, string path)
{
    cout<<"Save completed classes"<<endl;
    ostringstream ss;
    ss<<completedClasses.size();
    Mat vecCvt((int)completedClasses.size(),1,CV_32S);
    
    // Print completed classes set
    for(set<int>::iterator it=completedClasses.begin();it!=completedClasses.end();)
    {
        printf("Completed classes vector has value %d\n",*it);
        ++it;
    }
    
    // Copy completed classes to matrix
    int i=0;
    for(set<int>::iterator it=completedClasses.begin();it!=completedClasses.end();)
    {
        vecCvt.at<int>(i,0)=*it;
        ++it;
        ++i;
    }
    
    // Print matrix before save
    for(int i=0;i<completedClasses.size();i++)
    {
        printf("Completed classess matrix before save at location %d has value %d\n",i,vecCvt.at<int>(i,0));
    }
    
    // Save matrix to disk
    saveMat(vecCvt, path);
}

void loadCompletedClasses(set<int>& completedClasses, string path)
{
    cout<<"Load completed classes"<<endl;
    Mat vecCvt;
    
    // Load completed classes from disk
    loadMat(vecCvt, path);
    int nClasses=vecCvt.rows;
    ostringstream ss;
    ss<<nClasses;
    
    // Print matrix after load
    for(int i=0;i<nClasses;i++)
    {
        printf("Matrix after load at location %d has value %d\n",i,vecCvt.at<int>(i,0));
    }

    // Copy completed classes to set
    for(int i=0;i<nClasses;i++)
    {
        completedClasses.insert(vecCvt.at<int>(i,0));
    }
    
    // Print completed classes set
    for(set<int>::iterator it=completedClasses.begin();it!=completedClasses.end();)
    {
        printf("Completed classes vector has value %d\n",*it);
        ++it;
    }
}

void colorTabTest(int clusterCount, string dataDir)
{
    RNG rng(12345);
    vector<Scalar> colorTab;
    Scalar color;
    ostringstream ss;
    ss<<clusterCount;
    Mat vecCvt(clusterCount,1,CV_64FC4);
    
    // Create color tab
    for(int i=0;i<clusterCount;i++)
    {
        colorTab.push_back(Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255),0.0f));
    }
    
    // Print color tab
    for(int i=0;i<clusterCount;i++)
    {
        color = colorTab.at(i);
        printf("After Gen ColorTab(%d): (%0.2lf,%0.2lf,%0.2lf,%0.2lf)\n",i,colorTab.at(i)[0],colorTab.at(i)[1],colorTab.at(i)[2],colorTab.at(i)[3]);
    }
    
    // Copy color tab to matrix
    for(int i=0;i<clusterCount;i++)
    {
        for(int j=0;j<4;j++)
            vecCvt.at<Scalar>(i,0)[j]=colorTab.at(i)[j];
    }
    
    // Print matrix before save
    for(int i=0;i<clusterCount;i++)
    {
        printf("Matrix before save (%d): (%0.2lf,%0.2lf,%0.2lf,%0.2lf)\n",i,vecCvt.at<Scalar>(i,0)[0],vecCvt.at<Scalar>(i,0)[1],vecCvt.at<Scalar>(i,0)[2],vecCvt.at<Scalar>(i,0)[3]);
    }

    // Save matrix to disk
    saveMat(vecCvt, dataDir+"colorTab."+ss.str()+".bin");
    
    // Load color tab from disk
    loadMat(vecCvt, dataDir+"colorTab."+ss.str()+".bin");
    colorTab.resize(clusterCount);
    
    // Copy matrix to color tab
    for(int i=0;i<clusterCount;i++)
    {
        for(int j=0;j<4;j++)
            colorTab.at(i)[j]=vecCvt.at<Scalar>(i,0)[j];
    }
    
    // Print matrix after load
    for(int i=0;i<clusterCount;i++)
    {
        printf("Matrix after load (%d): (%0.2lf,%0.2lf,%0.2lf,%0.2lf)\n",i,vecCvt.at<Scalar>(i,0)[0],vecCvt.at<Scalar>(i,0)[1],vecCvt.at<Scalar>(i,0)[2],vecCvt.at<Scalar>(i,0)[3]);
    }
}

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

void createGraph3D(Mat& graph, Mat& labels, int clusterCount, string dataDir, bool bSaveState)
{
    RNG rng(rand());
    vector<Scalar> colorTab;
    Scalar color;
    ostringstream ss;
    ss<<clusterCount;
    Mat vecCvt(clusterCount,1,CV_64FC4);

    if(bSaveState)
    {
        // Create color tab
        for(int i=0;i<clusterCount;i++)
        {
            colorTab.push_back(Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255),0.0f));
        }

        // Copy color tab to matrix
        for(int i=0;i<clusterCount;i++)
        {
            for(int j=0;j<4;j++)
                vecCvt.at<Scalar>(i,0)[j]=colorTab.at(i)[j];
        }
        saveMat(vecCvt, dataDir+"colorTab."+ss.str()+".bin");
    }
    else
    {
        // Load color tab from disk
        loadMat(vecCvt, dataDir+"colorTab."+ss.str()+".bin");
        colorTab.resize(clusterCount);
        
        // Copy matrix to color tab
        for(int i=0;i<clusterCount;i++)
        {
            for(int j=0;j<4;j++)
                colorTab.at(i)[j]=vecCvt.at<Scalar>(i,0)[j];
        }
    }
    
    // Print color tab
    for(int i=0;i<clusterCount;i++)
    {
        color = colorTab.at(i);
        printf("ColorTab before assign (%d): (%0.2lf,%0.2lf,%0.2lf,%0.2lf)\n",i,colorTab.at(i)[0],colorTab.at(i)[1],colorTab.at(i)[2],colorTab.at(i)[3]);
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
                color = colorTab[clusterIdx];
                graph.data[row*(graph.step[0]) + col*channels] = color[0];
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

