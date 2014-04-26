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
    Mat points,centers,grayScale,mask;
    int clusterCount = 0;
    unsigned long sampleCount = 0;
    bool bSaveState=false;
    if(argc>=3 && strcmp(argv[2],"saveState")==0)
        bSaveState=true;
    
    // Generate image data
    Mat image = imread(dataDir+argv[1]+".png");
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
    TermCriteria termCrit=TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0);
    kmeans(points, clusterCount, pMouseInfo->labels, termCrit, 3, KMEANS_USE_INITIAL_LABELS, centers);
    for(int i=0;i<centers.rows;i++)
        printf("After Center(%d): (%0.2f,%0.2f,%0.2f)\n",i,centers.at<float>(i,0),centers.at<float>(i,1),centers.at<float>(i,3));
    if(bSaveState)
        saveMat(centers, dataDir+"centers."+ss.str()+".bin");
    
    // Display data clusters
    //scaleData2D(points, dataRange2D);
    //createGraph2D(graph, points, labels, dataRange2D, clusterCount, sampleCount);
    cout<<"Create graph cluster"<<endl;
    pMouseInfo->graph.create(image.rows,image.cols,CV_8UC3);
    createGraph3D(pMouseInfo->graph, pMouseInfo->labels, clusterCount, dataDir, bSaveState);
    char cn[256];
    sprintf(cn,"%s%s%s%d%s",dataDir.c_str(),argv[1],"Cluster",clusterCount,".png");
    imwrite(cn,pMouseInfo->graph);
    
    // Show graph and start defining background
    imshow("Clusters", pMouseInfo->graph);
    setMouseCallback("Clusters", onMouse, (void*) pMouseInfo);
    
    //cout<<"Create labels gray scale"<<endl;
    //grayScale.create(image.rows,image.cols,CV_8UC1);
    //createGraph3DGrayScale(grayScale, pMouseInfo->labels, clusterCount);
    //sprintf(cn,"%s%s%s%d%s",dataDir.c_str(),argv[1],"GrayScale",clusterCount,".png");
    //imwrite(cn,grayScale);
    //imshow("GrayScale", grayScale);

    printf("kmeans info  cluster count: %d sample count: %lu points size: (%d,%d) labels size: (%d,%d) centers size: (%d,%d)\n", clusterCount, sampleCount, points.rows, points.cols, pMouseInfo->labels.rows, pMouseInfo->labels.cols, centers.rows, centers.cols);
    
    waitKey();
    if(bSaveState)
    {
        saveCompletedClasses(pMouseInfo->completedClasses, dataDir+"completedClasses."+ss.str()+".bin");
    }
    else
    {
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
        // Do opening to get rid of speckles
        //erode(pMouseInfo->graph,pMouseInfo->graph,Mat());
        //dilate(pMouseInfo->graph,pMouseInfo->graph,Mat());

        imshow("Clusters", pMouseInfo->graph);
        waitKey();
    }
    
    // Convert graph with defined background to object map
    /*mask.create(image.rows,image.cols,CV_8UC1);
    mask=Scalar(255);
    int channels=mask.channels();
    imshow("mask1",mask);
    for(int i=0;i<pMouseInfo->labels.rows;i++)
    {
        for(set<int>::iterator it=pMouseInfo->completedClasses.begin();it!=pMouseInfo->completedClasses.end();)
        {
            if(pMouseInfo->labels.at<int>(i)==*it)
            {
                mask.data[i] = 0;
            }
            ++it;
        }
    }
    imshow("mask1",mask);*/
    
    // Convert to grayscale
    mask.create(image.rows,image.cols,CV_8UC1);
    cvtColor(pMouseInfo->graph, mask, CV_BGR2GRAY);
    imshow("Gray Scale",mask);
    waitKey();
    
    // Do opening to get rid of speckles
    erode(mask,mask,Mat());
    dilate(mask,mask,Mat());
    imshow("Gray Scale",mask);
    waitKey();
    
     // Do binary threshold
    threshold(mask, mask, 0, 255, THRESH_BINARY);
    imshow("Mask", mask);
    waitKey();
    
    // Convert float element to uchar
    sprintf(cn,"%s%s%s%d%s",dataDir.c_str(),argv[1],"Binary",clusterCount,".png");
    imwrite(cn,mask);
    
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
