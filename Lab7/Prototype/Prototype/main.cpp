#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include "Tracking.hpp"
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#define MIN_POINTS 2
#define BEGIN_RUN_FRAMES 15

using namespace cv;
using namespace std;
using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

bool circleDefined=false;
bool circleLocked=true;
bool pauseVid=true;
bool mouseMoving=false;
Point circleCenter=Point(-1,-1);
float radius=-1.0;
float userRadius=-1.0;
Mat mask,image,gray,prevGray;
vector<KeyPoint> keypoints;
vector<Point2f> points[2];
Size subPixWinSize(10,10), winSize(31,31);
TermCriteria termcrit(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03);
int minHessian = 400;
//SurfFeatureDetector detector( minHessian );
int maxSize=16, responseThreshold=10, lineThresholdProjected = 10, lineThresholdBinarized=8, suppressNonmaxSize=5;
StarFeatureDetector detector(maxSize, responseThreshold, lineThresholdProjected, lineThresholdBinarized, suppressNonmaxSize);
vector<Tracking> tracking;
int currentFrame=0;

static void onMouse( int event, int x, int y, int /*flags*/, void* /*param*/ );
int main(int argc, char* argv[])
{
    String dataDir="/Users/donj/workspace/cs585/Lab7/Data/";
    Mat image;
    size_t i, k;
    vector<uchar> status;

     //variables for capturing from the camera or using file input
    VideoCapture cap;
    
    //with no arguments, use the camera
    if( argc == 1 )
    {
        //with no arguments, use the camera
        cap.open(1);
        if( !cap.isOpened() )
        {
            cout << "Could not initialize webcam capturing...\n";
            return 0;
        }
    }
    else if(argc == 2)
    {
        cap.open(dataDir+argv[1]);
        if( !cap.isOpened() )
        {
            cout << "Could not initialize capturing...\n";
            return 0;
        }
    }
    else
    {
        cout<<"Incorrect number of program arguments "<<argc<<endl;
        return 1;
    }
    int nframes=cap.get(CV_CAP_PROP_FRAME_COUNT);
    int fps=cap.get(CV_CAP_PROP_FPS);
    for(int i=0;i<nframes;i++)
    {
        tracking.push_back(Tracking(fps, i));
    }
    cout<<"No. of Tracking objects in main: "<<tracking.size()<<endl;
    int curPos=cap.get(CV_CAP_PROP_POS_MSEC);
    printf("Total frames: %d FPS: %d Current Pos: %d\n",nframes,fps,curPos);
    
    //Initialize main window and set up mouse callback
    Mat frame;
    cap >> frame;
    if(!frame.empty() )
    {
        currentFrame++;
        frame.copyTo(image);
    }
    namedWindow("Main", WINDOW_AUTOSIZE);
    imshow("Main", image);
    setMouseCallback( "Main", onMouse, 0 );
    
    while (true)
    {
        char c = (char)waitKey(60);
        switch( c )
        {
            case 'p':
            case 'P':
                pauseVid=!pauseVid;
            case 'c':
            case 'C':
            case 27:
                points[0].clear();
                points[1].clear();
                circleDefined=false;
                break;
            case 'q':
            case 'Q':
                return 0;
        }
        if(!pauseVid || currentFrame < BEGIN_RUN_FRAMES)
        {
            //Read from the camera
            cap >> frame;
            if(frame.empty() )
                break;
            else
            {
                currentFrame++;
                frame.copyTo(image);
            }

            cvtColor(image, gray, COLOR_BGR2GRAY);
            mask=Mat(gray.rows, gray.cols, CV_8U,uint(0));
            
            if(!points[0].empty() && circleLocked)
            {
                vector<float> err;
                if(prevGray.empty())
                    gray.copyTo(prevGray);
                calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, winSize, 3, termcrit, 0, 0.001);
                for( i = k = 0; i < points[1].size(); i++ )
                {
                    if( !status[i] )
                        continue;
                    
                    points[1][k++] = points[1][i];
                    circle( image, points[1][i], 3, Scalar(0,255,0), -1, 8);
                    line(image, points[0][i], points[1][i], Scalar(0,255,0), 1, 8); //Diane's addition
                }
                points[1].resize(k);
                float xMin=FLT_MAX,xMax=-FLT_MAX,yMin=FLT_MAX,yMax=-FLT_MAX;
                for(vector<Point2f>::iterator it = points[1].begin();it !=points[1].end();it++)
                {
                    xMin = (*it).x < xMin ? (*it).x : xMin;
                    xMax = (*it).x > xMax ? (*it).x : xMax;
                    yMin = (*it).y < yMin ? (*it).y : yMin;
                    yMax = (*it).y > yMax ? (*it).y : yMax;
                }
                circleCenter=Point((int)((xMin+xMax)/2.0),(int)((yMin+yMax)/2.0));
                radius = (xMax-xMin)/2.0;
                radius = radius > (yMax-yMin)/2.0 ? radius : (yMax-yMin)/2.0;
                radius = max(radius,userRadius);
                tracking[currentFrame-1].setCircle(circleCenter, radius);
            }
            
            std::swap(points[1], points[0]);
            cv::swap(prevGray, gray);
        }
        if((circleDefined && (circleCenter.x != 0 || circleCenter.y != 0) && (points[1].size() > MIN_POINTS))
           || mouseMoving)
        {
            frame.copyTo(image);
            for( i = k = 0; i < points[1].size(); i++ )
            {
                if( !status[i] )
                    continue;
                
                points[1][k++] = points[1][i];
                circle( image, points[1][i], 3, Scalar(0,255,0), -1, 8);
                line(image, points[0][i], points[1][i], Scalar(0,255,0), 1, 8); //Diane's addition
            }
            circle(image,circleCenter, radius, Scalar(0,0,255), 1);
        }
        imshow("Main", image);
    }
    
    // Write json.
    ptree pt;
    pt.put ("foo", "bar");
    std::ostringstream buf;
    write_json (buf, pt, false);
    std::string json = buf.str(); // {"foo":"bar"}

	return 0;
}

static void onMouse( int event, int x, int y, int /*flags*/, void* /*param*/ )
{
    if( event == CV_EVENT_LBUTTONDOWN )
    {
        cout<<"Left button down"<<endl;
        pauseVid=true;
        circleLocked=false;
        circleDefined=false;
        circleCenter=Point(x,y);
        radius=0.0;
        points[0].empty();
        points[1].empty();
    }
    if( event == CV_EVENT_MOUSEMOVE && !circleLocked )
    {
        mouseMoving=true;
        radius=sqrt(pow(x-circleCenter.x,2)+pow(y-circleCenter.y,2));
        userRadius=radius;
        circleDefined=true;
        circle(image,circleCenter, radius, Scalar(0,0,255), 1);
    }
    if( event == CV_EVENT_LBUTTONUP )
    {
        cout<<"Left button up"<<endl;
        mouseMoving=false;
        circleLocked=true;
        mask.setTo(Scalar(0,0,0));
        //Detect the keypoints using SURF Detector
        circle(mask,circleCenter, radius, Scalar(255,255,255), -1);
        keypoints.clear();
        detector.detect(gray,keypoints,mask);
        points[0].clear();
        for(vector<KeyPoint>::iterator it = keypoints.begin();it !=keypoints.end();it++)
        {
            points[0].push_back((*it).pt);
        }
        if(!points[0].empty() )
        {
            vector<uchar> status;
            vector<float> err;
            if(prevGray.empty())
                gray.copyTo(prevGray);
            calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, winSize, 3, termcrit, 0, 0.001);
            size_t i, k;
            for( i = k = 0; i < points[1].size(); i++ )
            {
                if( !status[i] )
                    continue;
                
                points[1][k++] = points[1][i];
                circle( image, points[1][i], 3, Scalar(0,255,0), -1, 8);
                line(image, points[0][i], points[1][i], Scalar(0,255,0), 1, 8); //Diane's addition
            }
            points[1].resize(k);
            float xMin=FLT_MAX,xMax=-FLT_MAX,yMin=FLT_MAX,yMax=-FLT_MAX;
            for(vector<Point2f>::iterator it = points[1].begin();it !=points[1].end();it++)
            {
                xMin = (*it).x < xMin ? (*it).x : xMin;
                xMax = (*it).x > xMax ? (*it).x : xMax;
                yMin = (*it).y < yMin ? (*it).y : yMin;
                yMax = (*it).y > yMax ? (*it).y : yMax;
            }
            circleCenter=Point((int)((xMin+xMax)/2.0),(int)((yMin+yMax)/2.0));
            radius = (xMax-xMin)/2.0;
            radius = radius > (yMax-yMin)/2.0 ? radius : (yMax-yMin)/2.0;
            radius = max(radius,userRadius);
            cout<<"Current frame in onMouse: "<<currentFrame<<endl;
            cout<<"No. of Tracking objects in onMouse: "<<tracking.size()<<endl;
            tracking[currentFrame-1].setCircle(circleCenter, radius);
        }
        cout<<"No. of points: "<<points[0].size()<<endl;
        pauseVid=false;
    }
}

