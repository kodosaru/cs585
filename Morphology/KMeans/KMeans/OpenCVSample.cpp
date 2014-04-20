//
//  OpenCVSample.cpp
//  KMeans
//
//  Modifed by Don Johnson on 4/19/14. Originally take from OpenCV v2.7 sample library
//

#include "OpenCVSample.h"

using namespace cv;
using namespace std;

int main( int argc, char* argv[] )
{
    const int maxClusters = 20;
    Mat points,labels,centers,graph,grayScale;
    int clusterCount = 0;
    unsigned long sampleCount = 0;
    
    // Generate image data
    string dataDir="/Users/donj/workspace/cs585/Morphology/Data/Output/";
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
    TermCriteria termCrit=TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 20, 1.0);
    kmeans(points, clusterCount, labels, termCrit, 12, KMEANS_PP_CENTERS, centers);
    for(int i;i<centers.rows;i++)
        printf("Center(%d): (%0.2f,%0.2f,%0.2f)\n",i,centers.at<float>(i,0),centers.at<float>(i,1),centers.at<float>(i,3));
    
    // Display data clusters
    //scaleData2D(points, dataRange2D);
    //createGraph2D(graph, points, labels, dataRange2D, clusterCount, sampleCount);
    cout<<"Create graph cluster"<<endl;
    graph.create(image.rows,image.cols,CV_8UC3);
    createGraph3D(graph, labels, clusterCount);
    char cn[256];
    sprintf(cn,"%s%s%s%d%s",dataDir.c_str(),argv[1],"Cluster",clusterCount,".jpg");
    imwrite(cn,graph);
    imshow("Clusters", graph);
    
    cout<<"Create graph gray scale"<<endl;
    grayScale.create(image.rows,image.cols,CV_8UC1);
    createGraph3DGrayScale(grayScale, labels, clusterCount);
    sprintf(cn,"%s%s%s%d%s",dataDir.c_str(),argv[1],"GrayScale",clusterCount,".jpg");
    imwrite(cn,grayScale);
    imshow("GrayScale", grayScale);

    printf("kmeans info  cluster count: %d sample count: %lu points size: (%d,%d) labels size: (%d,%d) centers size: (%d,%d)\n", clusterCount, sampleCount, points.rows, points.cols, labels.rows, labels.cols, centers.rows, centers.cols);
    
    waitKey();

    return 0;
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

