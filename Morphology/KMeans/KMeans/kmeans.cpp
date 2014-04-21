//
//  OpenCVSample.cpp
//  KMeans
//
//  Modifed by Don Johnson on 4/19/14. Originally take from OpenCV v2.7 sample library
//

#include "kmeans.h"
#include "Methods.h"
#include "Archive.hpp"

using namespace cv;
using namespace std;

int main( int argc, char* argv[] )
{
    string dataDir="/Users/donj/workspace/cs585/Morphology/Data/Output/";
    const int maxClusters = 20;
    Mat points,labels,centers,graph,grayScale;
    int clusterCount = 0;
    unsigned long sampleCount = 0;
    bool bSaveState=false;
    if(argc>=3 && strcmp(argv[2],"saveState")==0)
        bSaveState=true;
    
    // Generate image data
    Mat image = imread(dataDir+argv[1]+".jpg");
    imshow("Image",image);
    int ucharMax=pow(2,sizeof(uchar)*8) - 1;
    Point3i dataRange3D(ucharMax,ucharMax,ucharMax);
    cout<<"Fill points array"<<endl;
    imageData(image, points, clusterCount, sampleCount, maxClusters);

    // Generate sample data
    //Point2i dataRange2D(500,500);
    //graph.create(Size(dataRange2D.y,dataRange2D.x), CV_8UC3);
    //generateSampleData(graph,points,centers,clusterCount,sampleCount,maxClusters);
    
    // Random number generator
    cout<<"Perform k-means clustering"<<endl;
    ostringstream ss;
    ss<<clusterCount;
    if(!bSaveState)
        loadMat(centers, dataDir+"centers."+ss.str()+".bin");
    for(int i=0;i<centers.rows;i++)
        printf("Before Center(%d): (%0.2f,%0.2f,%0.2f)\n",i,centers.at<float>(i,0),centers.at<float>(i,1),centers.at<float>(i,3));
    TermCriteria termCrit=TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 20, 1.0);
    kmeans(points, clusterCount, labels, termCrit, 1, KMEANS_USE_INITIAL_LABELS, centers);
    for(int i=0;i<centers.rows;i++)
        printf("After Center(%d): (%0.2f,%0.2f,%0.2f)\n",i,centers.at<float>(i,0),centers.at<float>(i,1),centers.at<float>(i,3));
    if(bSaveState)
        saveMat(centers, dataDir+"centers."+ss.str()+".bin");
    
    // Display data clusters
    //scaleData2D(points, dataRange2D);
    //createGraph2D(graph, points, labels, dataRange2D, clusterCount, sampleCount);
    cout<<"Create graph cluster"<<endl;
    graph.create(image.rows,image.cols,CV_8UC3);
    createGraph3D(graph, labels, clusterCount, dataDir, bSaveState);
    char cn[256];
    sprintf(cn,"%s%s%s%d%s",dataDir.c_str(),argv[1],"Cluster",clusterCount,".jpg");
    imwrite(cn,graph);
    imshow("Clusters", graph);
    
    cout<<"Create graph gray scale"<<endl;
    grayScale.create(image.rows,image.cols,CV_8UC1);
    createGraph3DGrayScale(grayScale, labels, clusterCount);
    sprintf(cn,"%s%s%s%d%s",dataDir.c_str(),argv[1],"GrayScale",clusterCount,".jpg");
    imwrite(cn,grayScale);
    //imshow("GrayScale", grayScale);

    printf("kmeans info  cluster count: %d sample count: %lu points size: (%d,%d) labels size: (%d,%d) centers size: (%d,%d)\n", clusterCount, sampleCount, points.rows, points.cols, labels.rows, labels.cols, centers.rows, centers.cols);
    
    waitKey();

    return 0;
}

