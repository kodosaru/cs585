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

using namespace cv;
using namespace std;

/*bool circleDefined=false;
bool circleLocked=true;
Point circleCenter=Point(0,0);
int radius=0;
Mat mask;

static void onMouse( int event, int x, int y, int , void* );
int main(int argc, char* argv[])
{
    int currentFrame=0;
    String dataDir="/Users/donj/workspace/cs585/Lab7/Data/";
    const int MAX_COUNT = 500;
    Mat image;
    TermCriteria termcrit(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03);
    Size subPixWinSize(10,10), winSize(31,31);
    bool needToInit=true;
    
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
    int curPos=cap.get(CV_CAP_PROP_POS_MSEC);
    printf("Total frames: %d FPS: %d Current Pos: %d\n",nframes,fps,curPos);
    
    namedWindow( "Main", WINDOW_AUTOSIZE);
    
    //The mouse callback will allow us to initialize a template in the window
    setMouseCallback( "Main", onMouse, 0 );
    
    Mat frame, prevGray, gray;
    vector<Point2f> points[2];
    vector<KeyPoint> keypoints;
    while (true)
    {
        currentFrame++;
        
        //Read from the camera
        cap >> frame;
        if(frame.empty() )
            break;
        frame.copyTo(image);
        
        cvtColor(image, gray, COLOR_BGR2GRAY);
        
        if(needToInit)
        {
            needToInit=false;
            void cvGoodFeaturesToTrack(const CvArr* image, CvArr* eig_image, CvArr* temp_image, CvPoint2D32f* corners, int* corner_count, double quality_level, double min_distance, const CvArr* mask=NULL, int block_size=3, int use_harris=0, double k=0.04 )
             
             Parameters:
             image – Input 8-bit or floating-point 32-bit, single-channel image.
             eig_image – The parameter is ignored.
             temp_image – The parameter is ignored.
             corners – Output vector of detected corners.
             maxCorners – Maximum number of corners to return. If there are more corners than are found, the strongest of them is returned.
             qualityLevel – Parameter characterizing the minimal accepted quality of image corners. The parameter value is multiplied by the best corner quality measure, which is the minimal eigenvalue (see cornerMinEigenVal() ) or the Harris function response (see cornerHarris() ). The corners with the quality measure less than the product are rejected. For example, if the best corner has the quality measure = 1500, and the qualityLevel=0.01 , then all the corners with the quality measure less than 15 are rejected.
             minDistance – Minimum possible Euclidean distance between the returned corners.
             mask – Optional region of interest. If the image is not empty (it needs to have the type CV_8UC1 and the same size as image ), it specifies the region in which the corners are detected.
             blockSize – Size of an average block for computing a derivative covariation matrix over each pixel neighborhood. See cornerEigenValsAndVecs() .
             useHarrisDetector – Parameter indicating whether to use a Harris detector (see cornerHarris()) or cornerMinEigenVal().
             k – Free parameter of the Harris detector.
            mask=Mat(gray.rows, gray.cols, CV_8U,uint(0));
            mask(Rect(100,100,300,300)) = 1;
            goodFeaturesToTrack(gray, points[1], MAX_COUNT, 0.01, 10, gray, 3, 0, 0.04);
            
            //-- Step 1: Detect the keypoints using SURF Detector
            int minHessian = 400;
            SurfFeatureDetector detector( minHessian );
            detector.detect(image,keypoints,mask);
            
            void cornerSubPix(InputArray image, InputOutputArray corners, Size winSize, Size zeroZone, TermCriteria criteria)
             
             Parameters:
             image – Input image.
             corners – Initial coordinates of the input corners and refined coordinates provided for output.
             winSize – Half of the side length of the search window. For example, if winSize=Size(5,5) , then a 5*2+1 \times 5*2+1 = 11 \times 11 search window is used.
             zeroZone – Half of the size of the dead region in the middle of the search zone over which the summation in the formula below is not done. It is used sometimes to avoid possible singularities of the autocorrelation matrix. The value of (-1,-1) indicates that there is no such a size.
             criteria – Criteria for termination of the iterative process of corner refinement. That is, the process of corner position refinement stops either after criteria.maxCount iterations or when the corner position moves by less than criteria.epsilon on some iteration.
            cornerSubPix(gray, points[1], subPixWinSize, Size(-1,-1), termcrit);
        }
        
        //-- Draw keypoints
        //drawKeypoints( image, keypoints, image, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
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
        }
        
        if(circleDefined)
            circle(image,circleCenter, radius, Scalar(0,0,255), 1);
        
        imshow("Main", image);
        
        char c = (char)waitKey(60);
        switch( c )
        {
            case 27:
                circleDefined=false;
                circleCenter=Point(0,0);
                radius=0;
                break;
            case 'r':
                needToInit = true;
                break;
            case 'c':
                points[0].clear();
                points[1].clear();
                break;
            case 'q':
                return 0;
        }
        std::swap(points[1], points[0]);
        //std::swap(keypoints[1], keypoints[0]);
        cv::swap(prevGray, gray);
    }
    
    
	return 0;
}

static void onMouse( int event, int x, int y, int, void*)
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
        circle(mask,circleCenter, radius, Scalar(255,255,255), -1);
    }
}

*/
