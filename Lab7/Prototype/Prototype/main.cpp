#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <string>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;
Point circleCenter=Point(0,0);
int radius=0;
Mat image;
bool circleDefined=false;
bool circleLocked=true;

static void onMouse( int event, int x, int y, int /*flags*/, void* /*param*/ );
int main(int argc, char* argv[])
{
    int currentFrame=0;
    String dataDir="/Users/donj/workspace/cs585/Lab7/Data/";
    
     //variables for capturing from the camera or using file input
    VideoCapture cap;
    
    
    //with no arguments, use the camera
    if( argc == 1 )
    {
        //with no arguments, use the camera
        cap.open(0);
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
    int curPos=cap.get(CV_CAP_PROP_POS_MSEC);
    printf("Total frames: %d FPS: %d Current Pos: %d\n",nframes,fps,curPos);
    
    namedWindow( "Main", WINDOW_AUTOSIZE);
    
    //The mouse callback will allow us to initialize a template in the window
    setMouseCallback( "Main", onMouse, 0 );
    
    Mat frame;
    while (true)
    {
        currentFrame++;
        
        //Read from the camera
        cap >> frame;
        if(frame.empty() )
            break;
        else
            frame.copyTo(image);
        
        if(circleDefined)
            circle(image,circleCenter, radius, Scalar(0,0,255), 1);
        imshow("Main", image);
        
        char c = (char)waitKey(60);
        if( c == 27 )
        {
            circleDefined=false;
            circleCenter=Point(0,0);
            radius=0;
        }
        
        if( c == 'q' )
        {
            break;
        }
        
    }
    
	return 0;
}

static void onMouse( int event, int x, int y, int /*flags*/, void* /*param*/ )
{
    if( event == CV_EVENT_LBUTTONDOWN )
    {
        circleLocked=false;
        circleDefined=false;
        circleCenter=Point(x,y);

    }
    if( event == CV_EVENT_MOUSEMOVE && !circleLocked )
    {
        radius=sqrt(pow(x-circleCenter.x,2)+pow(y-circleCenter.y,2));
        circleDefined=true;
    }
    if( event == CV_EVENT_LBUTTONUP )
    {
        circleLocked=true;
    }

}

