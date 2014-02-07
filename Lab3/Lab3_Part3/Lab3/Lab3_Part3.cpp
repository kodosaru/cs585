// OpenCVWebcam.cpp : Defines the entry point for the console application.
//

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>

using namespace cv;
using namespace std;


int main(int argc, char* argv[])
{
    VideoCapture capture;
    capture.open(0);
    int error;
    if(!capture.isOpened())
    {
        error = -1;
        return 1;
    }

    namedWindow( "Camera View", 1 );
    // http://docs.opencv.org/doc/tutorials/objdetect/cascade_classifier/cascade_classifier.html

    Mat view;
    //bool blink = false;

    bool bRecording = false;
    int frameNumber=0;
    char directory[128]={'\0'};
    directory[0]='.';
    strcpy(directory,"/Users/donj/workspace/cs585/Lab3/trackingSequence");
    if(argc > 1)
    {
        strcpy(directory, argv[1]);
    }
    char filename[256];

    while( capture.isOpened() )
    {
        Mat view0;
        capture.read(view0);
        view0.copyTo(view);
        imshow("Camera View", view);
        if(bRecording)
        {
            sprintf(filename, "%s/video_%04d.jpg", directory, frameNumber);
            imwrite(filename  , view);
            frameNumber++;
        }

        char key = waitKey(33);
        if(key == 'p')
        {
            sprintf(filename, "%s/video_%04d.jpg", directory, frameNumber);
            imwrite(filename, view);
            frameNumber++;
        }
        if(key == ' ')
        {
            bRecording = !bRecording;
        }
        if(key == 'q')
        {
            break;
        }
    }

    return 0;
}

