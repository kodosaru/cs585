// OpenCVWebcam.cpp : Defines the entry point for the console application.
//

#include "Methods.h"
#include "TrackedObject.h"

//a dummy function to pass to the slider bar to threshold the red object
void onTrackbar(int value, void* data);

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
    int areaThreshold = 400;
    bool bTracking = false;
    vector<TrackedObject> tracks;

    //for recording frames of video and results
    bool bRecordVideo = false;
    bool bRecordResults = false;
    int frameNumber=0;
    
    vector<Point> detections;
    vector<Mat> imageBuffer(4);
    //detectionsBuffer is a vector, one per image, of a vector of points, size up to MAX_OBJECTS
    vector<vector<Point>*> detectionsBuffer(MAX_IMAGES);
    for(int i=0;i<MAX_IMAGES;i++)
    {
        detectionsBuffer[i]=new vector<Point>();
        for(int j=0;j<MAX_OBJECTS;j++)
        {
            detectionsBuffer[i]->push_back(Point(0,0));
        }
    }


    //create the window with a trackbar for the slider
    namedWindow( "Camera View", 1 );
    createTrackbar( "Red Threshold", "Camera View", &redThreshold, 255, onTrackbar );
    createTrackbar( "Area Threshold", "Camera View", &areaThreshold, 1000, onTrackbar );

    while( capture.isOpened() )
    {
        //shuffle images; newest has lowest indice;imageBuffer[0]
        for(int i=0;i<MAX_IMAGES-1;i++)
            imageBuffer[i+1]=imageBuffer[i];
        //empty most current image buffer to get ready for capture below
        imageBuffer[0].empty();
        
        //save vector of detection points pointer associated with oldest image
        vector<Point>* pVect=detectionsBuffer[MAX_IMAGES-1];
        //shuffle detections vectors so they correspond to images in image buffer
        for(int i=0;i<MAX_IMAGES-1;i++)
        {
            detectionsBuffer[i+1]=detectionsBuffer[i];
        }
        //reuse saved vector of points
        detectionsBuffer[0]=pVect;
        
        capture.read(imageBuffer[0]);
        if(bRecordVideo)
        {
            sprintf(filename, "video/video_%04d.jpg", frameNumber);
            imwrite(filename, imageBuffer[0]);
        }

        if(bTracking)
        {
            //We are currently tracking the red object, so draw the track so the user can see it
            trackRedObjects(imageBuffer[0], tracks, detections, detectionsBuffer, redThreshold, areaThreshold);
            if(DEBUG && detections.size()>MAX_OBJECTS)
                printf("Maximum no. of objects exceeded %ld > %d\n",detections.size(),MAX_OBJECTS);
                
            for(int i=0;i<detections.size();i++)
            {
                //save detections associated with Image 0 to detectionsBuffer vector 0
                detectionsBuffer[0]->at(i)=Point(detections[i].x,detections[i].y);
            }
            
            //print detections vectors associated with each image
            if(DEBUG)
            {
                if(detectionsBuffer.size()!=MAX_IMAGES)
                {
                    printf("detectionsBuffer size %ld not equal MAX_IMAGES %d\n",detectionsBuffer.size(),MAX_IMAGES);
                    return 1;
                }
                for(int i=0;i<MAX_IMAGES;i++)
                {
                    vector<Point> tvec = *(detectionsBuffer[i]);
                    int nsize = (int)tvec.size();

                    if(nsize != MAX_OBJECTS)
                    {
                        printf("tvec size %d not equal MAX_OBJECTS %d\n",nsize,MAX_OBJECTS);
                        return 1;
                    }

                    if(nsize>0)
                    {
                        printf("Image %d detections: ",i);
                        for(int j=0;j<nsize-1;j++)
                            printf("(%d,%d),",tvec[j].x,tvec[j].y);
                        printf("(%d,%d)",tvec[nsize-1].x,tvec[nsize-1].y);
                        cout<<endl;
                    }
                }
            }
            

            //draw the tracks
            for(int t=0; t<tracks.size(); t++)
            {
                drawLine(imageBuffer[0], tracks[t].history, tracks[t].color);
            }
        }

        if(bRecordResults)
        {
            sprintf(filename, "%sresults/results_%04d.jpg",dataDir.c_str(),frameNumber);
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