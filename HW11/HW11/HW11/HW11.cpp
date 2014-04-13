// OpenCVWebcam.cpp : Defines the entry point for the console application.
//

#include "Methods.h"
#include "TrackedObject.h"
#define MAX_IMAGES 4
#define MAX_OBJECTS 3

//a dummy function to pass to the slider bar to threshold the red object
void onTrackbar(int value, void* data);

unsigned long nDetections=0;

int main(int argc, char* argv[])
{
    String dataDir="/Users/donj/workspace/cs585/HW11/Data/";
    VideoCapture capture;
    char filename[256];
    capture.open(0);
    if(!capture.isOpened())
    {
        return 1;
    }

    //for tracking the balls
    int redThreshold=153;
    int areaThreshold = 200;
    bool bTracking = false;
    vector<TrackedObject> tracks;

    //for recording frames of video and results
    bool bRecordVideo = false;
    bool bRecordResults = false;
    int frameNumber=0;
    
    vector<Point> detections;
    vector<Mat> imageBuffer(4);
    vector<vector<Point>*> detectionsBuffer(3);
    for(int i=0;i<MAX_OBJECTS;i++)
        detectionsBuffer[i]=new vector<Point>();

    //create the window with a trackbar for the slider
    namedWindow( "Camera View", 1 );
    createTrackbar( "Red Threshold", "Camera View", &redThreshold, 255, onTrackbar );
    createTrackbar( "Area Threshold", "Camera View", &areaThreshold, 1000, onTrackbar );

    while( capture.isOpened() )
    {
        for(int i=0;i<MAX_IMAGES-1;i++)
            imageBuffer[i+1]=imageBuffer[i];
        for(int i=0;i<MAX_OBJECTS;i++)
        {
            vector<Point> tvec = *(detectionsBuffer[i]);
            for(int j=0;j<(int)tvec.size()-1;j++)
            {
                tvec[j+1]=tvec[j];
            }
        }
        for(int i=0;i<MAX_OBJECTS;i++)
        {
            vector<Point> tvec = *(detectionsBuffer[i]);
            int nsize = (int)tvec.size();
            if(nsize>0)
            {
                printf("Object %d detections: ",i);
                for(int j=0;j<nsize-1;j++)
                    printf("(%d,%d),",tvec[j].x,tvec[j].y);
                printf("(%d,%d)",tvec[nsize-1].x,tvec[nsize-1].y);
                cout<<endl;
            }
        }

        capture.read(imageBuffer[0]);
        if(bRecordVideo)
        {
            sprintf(filename, "video/video_%04d.jpg", frameNumber);
            imwrite(filename, imageBuffer[0]);
        }

        if(bTracking)
        {
            //We are currently tracking the red object, so draw the track so the user can see it
            trackRedObjects(imageBuffer[0], tracks, detections, redThreshold, areaThreshold);
            //draw the tracks
            for(int t=0; t<tracks.size(); t++)
            {
                drawLine(imageBuffer[0], tracks[t].history, tracks[t].color);
            }
        }

        if(bRecordResults)
        {
            sprintf(filename, "results/results_%04d.jpg", frameNumber);
            imwrite(filename, imageBuffer[0]);
        }



        imshow("Camera View", imageBuffer[0]);
        char key = waitKey(33);
        if(key == 'q')
        {
            break;
        }
        if(key == 't')
        {
            bTracking = !bTracking;

            //We are starting our tracking, so clear out what was in there before
            if(bTracking) 
            {
                tracks.clear();
            }
        }
        if(key == 'v')
        {
            bRecordVideo = !bRecordVideo;
        }
        if(key == 'r')
        {
            bRecordResults = !bRecordResults;
        }
        if(key == ' ')
        {
            bRecordResults = !bRecordResults;
            bRecordVideo = bRecordResults;
        }

        if(bRecordResults || bRecordVideo)
        {
            frameNumber++;
        }
    }

    return 0;
}

void onTrackbar(int value, void* data)
{
}