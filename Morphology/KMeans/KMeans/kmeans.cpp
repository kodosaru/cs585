//
//  KMeans
//
//  Modifed by Don Johnson on 4/19/14. Originally taken from OpenCV v2.7 sample library
//

#include "kmeans.h"

using namespace cv;
using namespace std;

int main( int argc, char* argv[] )
{
    MOUSEINFO mouseInfo,*pMouseInfo;
    pMouseInfo=&mouseInfo;
    string dataDir="/Users/donj/workspace/cs585/Morphology/Data/Output/";
    const int maxClusters = 20;
    Mat points,centers,grayScale,objects;
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
    kmeans(points, clusterCount, pMouseInfo->labels, termCrit, 1, KMEANS_USE_INITIAL_LABELS, centers);
    for(int i=0;i<centers.rows;i++)
        printf("After Center(%d): (%0.2f,%0.2f,%0.2f)\n",i,centers.at<float>(i,0),centers.at<float>(i,1),centers.at<float>(i,3));
    if(bSaveState)
        saveMat(centers, dataDir+"centers."+ss.str()+".bin");
    
    // Display data clusters
    //scaleData2D(points, dataRange2D);
    //createGraph2D(graph, points, labels, dataRange2D, clusterCount, sampleCount);
    cout<<"Create graph cluster"<<endl;
    pMouseInfo->graph.create(image.rows,image.cols,CV_8UC3);
    objects.create(image.rows,image.cols,CV_8UC1);
    createGraph3D(pMouseInfo->graph, pMouseInfo->labels, clusterCount, dataDir, bSaveState);
    char cn[256];
    sprintf(cn,"%s%s%s%d%s",dataDir.c_str(),argv[1],"Cluster",clusterCount,".jpg");
    imwrite(cn,pMouseInfo->graph);
    imshow("Clusters", pMouseInfo->graph);
    setMouseCallback("Clusters", onMouse, (void*) pMouseInfo);
    
    cout<<"Create graph gray scale"<<endl;
    grayScale.create(image.rows,image.cols,CV_8UC1);
    createGraph3DGrayScale(grayScale, pMouseInfo->labels, clusterCount);
    sprintf(cn,"%s%s%s%d%s",dataDir.c_str(),argv[1],"GrayScale",clusterCount,".jpg");
    imwrite(cn,grayScale);
    //imshow("GrayScale", grayScale);

    printf("kmeans info  cluster count: %d sample count: %lu points size: (%d,%d) labels size: (%d,%d) centers size: (%d,%d)\n", clusterCount, sampleCount, points.rows, points.cols, pMouseInfo->labels.rows, pMouseInfo->labels.cols, centers.rows, centers.cols);
    
    waitKey();
    saveCompletedClasses(pMouseInfo->completedClasses, dataDir+"completedClasses."+ss.str()+".bin");
    loadCompletedClasses(pMouseInfo->completedClasses, dataDir+"completedClasses."+ss.str()+".bin");
    
    return 0;
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
        cout << "Point ("<<x<<","<<y<<")"<<endl;
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
