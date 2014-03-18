//CS585 HW7
//For this homework, we will try our hand at using template tracking

#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Functions.hpp"
#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;

Point2f point;
bool addRemovePt = false;
int patchSize=50; //the size of the image patches
int searchRadius=100; //the size of the search radius
Size imageSize;
bool displayMatch=false;

static void onMouse( int event, int x, int y, int /*flags*/, void* /*param*/ );

int main( int argc, char** argv )
{
    String dataDir="/Users/donj/workspace/cs585/Lab7/Data/";
        
    //variables for capturing from the camera or using file input
    VideoCapture cap;
    char directory[256], filename[256];
    int startFrame(-1), endFrame (-1);
    int currentFrame = 1;
    int frameNumber=0;
    bool bRecording = false;
    addRemovePt = false;

    if( argc == 1 )
    {
        //with no arguments, use the camera
        cap.open(0);
        if( !cap.isOpened() )
        {
            cout << "Could not initialize capturing...\n";
            return 0;
        }
    }
    else if (argc == 4)
    {
        //otherwise, use a sequence of images (as in HW3)
        //You can collect an image sequence using the code from Lab3_Part3
        strcpy(directory, argv[1]);
        startFrame = atoi(argv[2]);
        endFrame = atoi(argv[3]);
        currentFrame = startFrame;
    }
    else
    {
        cout<<"Wrong number of arguments"<<endl;
        return 0;
    }

    namedWindow( "Gray", 1 );
    namedWindow( "Template Tracking", 1 );

    //The mouse callback will allow us to initialize a template in the window
    setMouseCallback( "Template Tracking", onMouse, 0 );

    Mat gray, displayGray, image;
    vector<Point2f> points; //for the locations of the templates
    vector<Mat> patches; //for the image patches

    addRemovePt = false; //this is so the mouse callback event can tell us when to add points

    for(;;)
    {
        currentFrame++;

        //if we're using an image sequence and we're at the end, stop
        if(endFrame > 0 && currentFrame > endFrame)
        {
            break;
        }

        Mat frame;
        if(cap.isOpened())
        {
            //Read from the camera
            cap >> frame;
            cap.set(CV_CAP_PROP_FPS, 0.1);
            if( frame.empty() )
                break;
            frame.copyTo(image);
            imageSize=image.size();
        }
        else
        {
            //Read from a file
            sprintf(filename, "%s/video_%04d.jpg", directory, currentFrame);
            image = imread(filename);
        }


        if(bRecording)
        {
            //Record the original image if necessary
            //sprintf(filename, "video_%04d.jpg", currentFrame);
            //imwrite(filename, image);
        }

        //convert to grayscale
        cvtColor(image, gray, COLOR_BGR2GRAY);
        gray.copyTo(displayGray);

        if( addRemovePt)
        {
            //assume that the user wants to click in the middle of the patch, but we'll store
            //the upper-left corner to reduce confusion with matchTemplate in the trackTemplate function
            point.x = point.x - patchSize/2;
            point.y = point.y - patchSize/2;
            cout<<"\tPush point"<<endl;
            points.push_back(point);

            // http://stackoverflow.com/questions/10137766/copying-region-of-an-image-to-another-region-in-another-image
            // Got to be careful to actually copy the data out, otherwise the 
            // template will share memory with the gray image, and the data underlying the template 
            // will keep changing and cause mysterious bugs
            Rect ROI(point.x, point.y, patchSize, patchSize);
            Mat patch = gray(ROI);
            cout<<"\tPush patch"<<endl;
            patches.push_back(Mat());
            patch.copyTo(patches.back());

            //reset state so we don't add many redundant points
            addRemovePt = false;
        }

        //Go through our list of templates and track each of them
        if( !points.empty() )
        {
            for(int i=0; i<points.size(); i++)
            {
                //track the template
                trackTemplate(points[i], patches[i], gray, displayGray, i, searchRadius);
                
                //draw a rectangle around its location
                rectangle(image, points[i], points[i]+Point2f(patchSize, patchSize), Scalar(0,255,0), 2, 8, 0);
            }
        }

        //save the finished image if necessary
        if(bRecording)
        {
            sprintf(filename, "HW7_Part1_TemplateTracking_result_%04d.jpg", frameNumber++);
            imwrite(dataDir+filename, displayGray);
        }

        imshow("Gray", displayGray);
        imshow("Template Tracking", image);
 
        char c = (char)waitKey(60);
        if( c == 27 )
            break;
        
        if( c == 'c' )
        {
            points.clear();
            patches.clear();
        }

        if( c == 'q' )
        {
            break;
        }
        if( c == ' ')
        {
            bRecording = !bRecording;
        }

    }

    return 0;
}


static void onMouse( int event, int x, int y, int /*flags*/, void* /*param*/ )
{
    if( event == CV_EVENT_LBUTTONDOWN )
    {
        if((x - patchSize/2 - searchRadius) > 0 && (y - patchSize/2 - searchRadius) > 0
           && (x + patchSize/2 + searchRadius) < imageSize.width && (y + patchSize/2 + searchRadius < imageSize.height))
        {
            cout << "Mouse callback function" << endl;
            point = Point2f((float)x, (float)y);
            addRemovePt = true;
        }
        else
        {
            cout<<"Point too close to image border"<<endl;
        }
    }
}

