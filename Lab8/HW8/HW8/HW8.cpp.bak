// OpenCVWebcam.cpp : Defines the entry point for the console application.
//
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#define USE_MATH_DEFINES 1
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "Functions.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    VideoCapture capture;
    //capture.open(0); //to capture from live camera
    if(argc > 1)
    {
        //capture.open("../eggsOnTowers.mov");
        capture.open(argv[1]);
    }
    else
    {
        capture.open(0);
    }

    if(!capture.isOpened())
    {
        int error = -1;
        return 1;
    }

    namedWindow( "Flow Magnitude", 1 );
    namedWindow( "Flow Angle", 1 );
    namedWindow( "Camera View", 1 );

    Mat view, magnitudeView, angleView;
    Mat gray1, gray2;
    Mat flow, magnitude, angle;
    vector<Mat> view_record(5), magnitude_record(5), angle_record(5);
    int nFrame[N_SAVE_FRAMES];
    bool blink = false;

    bool bRecording = false;
    int frameNumber=0;
    char directory[256];
    strcpy(directory, "/Users/donj/workspace/cs585/Lab8/Data/");
    int fcount=0;

    if(argc > 1)
    {
        strcpy(directory, argv[1]);
    }
    char filename[256];

    //can copy from HW7_Part1 to use a sequence of jpg files instead of a video or the camera

    while( capture.isOpened())
    {
        Mat view0;
        if(!capture.read(view0))
        {
            break;
        }
        if(view0.cols > 1000) //video from my phone is 1080 x 1960, which is too big for slow optical flow
        {
            resize(view0, view, Size(0,0), .5, .5);
        }
        else
        {
            view0.copyTo(view);
        }
        cvtColor(view, gray2, COLOR_BGR2GRAY); //optical flow works on grayscale images

        if(frameNumber > 1)
        {
            // copied from OpenCV sample fback.cpp
            calcOpticalFlowFarneback(gray1, gray2, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
            drawOptFlowMap(flow, view, 16, 1.5, Scalar(0, 255, 0));

            //You need to fill in these functions
            //cout<<"Calc flow mag and angle for frame "<<frameNumber<<endl;
            opticalFlowMagnitudeAngle(flow, magnitude, angle, 1);
            visualizeFlow(magnitude, angle, magnitudeView, angleView);
            imshow("Flow Magnitude", magnitudeView);
            imshow("Flow Angle", angleView);
        }
        imshow("Camera View", view);

        char key = waitKey(33);
        if(key == 'p')
        {
            sprintf(filename, "%s/video_%04d.jpg", directory, frameNumber);
            imwrite(filename, view);
            sprintf(filename, "%s/magnitude_%04d.jpg", directory, frameNumber);
            imwrite(filename, magnitudeView);
            sprintf(filename, "%s/angle_%04d.jpg", directory, frameNumber);
            imwrite(filename, angleView);
            frameNumber++;
        }
        if(bRecording)
        {
                recordFrames(view, magnitudeView, angleView, view_record, magnitude_record, angle_record, bRecording, frameNumber, nFrame, fcount);
        }
        
        if(key == 'w')
        {
            writeFrames(view_record, magnitude_record, angle_record, directory, nFrame);
        }
        
        if(key == ' ')
        {
            bRecording = !bRecording;
            fcount=0;
        }
        
        if(key == 'q')
        {
            break;
        }
        
        gray2.copyTo(gray1);
        frameNumber++;
    }
    
    return 0;
}

