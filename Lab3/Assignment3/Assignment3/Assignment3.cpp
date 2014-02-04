// OpenCVWebcam.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <string>
#include <vector>

using namespace cv;
using namespace std;

 //function to detect faces after normalizing the image
 void detectFaces( Mat& image, CascadeClassifier& cascade, vector<Rect>& faces );

 //functions to detect the largest red object in an image
 bool findLargestRedObject(Mat& view, Point& location, vector<Point>& outline, int redThreshold);

//function to track the red object
void trackRedObject(Mat& image, vector<Point>& track, int redThreshold);

//a dummy function to pass to the slider bar to threshold the red object
void onTrackbar(int value, void* data);

// Compute the area and center of a region bounded by an outline
void computeObjectAreaAndCenter(vector<Point>& outline, double& area, Point& center);

//Draw an outline stored in a vector of points
void drawOutline(Mat& image, vector<Point>& outline);

//Translate (move) an outline stored in a vector of points by adding an offset to 
//the coordinates of all points in the outline
void translateOutline(vector<Point>& outline, Point center);

//Scale an outline by multiplying all coordinates of all points in the outline by a constant
void scaleOutline(vector<Point>& outline, double scale);


int main(int argc, char* argv[])
{
    VideoCapture capture;
    char filename[256];
    capture.open(0);
    if(!capture.isOpened())
    {
        int error = -1;
        return 1;
    }

   //Load face the cascades
   string face_cascade_name = "haarcascade_frontalface_alt.xml";
   CascadeClassifier face_cascade;
   if( !face_cascade.load( face_cascade_name ) )
   {
       printf("--(!)Error loading\n"); return -1; 
   }

    //for tracking the ball
    vector<Point> drawing; //to keep track of the original track
    vector<Point> outline; //for the version that we are drawing (scaled and translated)
    double drawingArea;
    Point drawingCenter;
    int redThreshold=190;
    bool bTracking = false;

    //for recording frames of video and results
    bool bRecordVideo = false;
    bool bRecordResults = false;
    int frameNumber=0;

    //create the window with a trackbar for the slider for the red threshold
    namedWindow( "Camera View", 1 );
    createTrackbar( "Red Threshold", "Camera View", &redThreshold, 255, onTrackbar );

    Mat view, view0;
    capture.read(view0);
    view0.copyTo(view);

    bool blink = false;

    while( capture.isOpened() )
    {
        capture.read(view0);
        view0.copyTo(view);
        if(bRecordVideo)
        {
            sprintf_s(filename, "video/video_%04d.jpg", frameNumber);
            imwrite(filename, view);
        }

        if(bTracking)
        {
            //We are currently tracking the red object, so draw the track so the user can see it
            trackRedObject(view, drawing, redThreshold);
            //draw the outline as drawn
            drawOutline(view, drawing);
        }
        else if(drawing.size() > 0)
        {
            //If we are not tracking, then detect the faces and draw the outline around the faces
            std::vector<Rect> faces;
            detectFaces(view, face_cascade, faces);
            for(int F=0; F<faces.size(); F++)
            {
                //calculate the center of the faces returned by the face detector
                Point faceCenter (faces[F].x+faces[F].width/2, faces[F].y + faces[F].height/2);

                // copy the drawing into another vector so we can manipulate it
                outline = drawing;

                // Required: draw the user's drawing around the face
            }
        }


        if(bRecordResults)
        {
            sprintf_s(filename, "results/results_%04d.jpg", frameNumber);
            imwrite(filename, view);
        }



        imshow("Camera View", view);
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
                drawing.clear();
            }
            else //We are all done tracking, so do a little post-processing
            {
                //get basic information about the drawing that the user drew
                computeObjectAreaAndCenter(drawing, drawingArea, drawingCenter);
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

void drawOutline(Mat& image, vector<Point>& outline)
{
    int numPoints = outline.size()-1;
    for(int f=0; f<numPoints; f++)
    {
        line(image, outline[f], outline[f+1], Scalar(255, 0, 0), 3);
    }
}

void translateOutline(vector<Point>& outline, Point center)
{
}

void scaleOutline(vector<Point>& outline, double scale)
{
}

void trackRedObject(Mat& view, vector<Point>& track, int redThreshold)
{
}

void computeObjectAreaAndCenter(vector<Point>& outline, double& area, Point& center)
{
    // http://docs.opencv.org/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html
    Moments objectProperties;
    objectProperties = moments(outline, false);

    area = objectProperties.m00;
    center.x=(objectProperties.m10/area);
    center.y=(objectProperties.m01/area);
}

void detectFaces( Mat& image, CascadeClassifier& face_cascade, vector<Rect>& faces )
{

// http://docs.opencv.org/modules/objdetect/doc/cascade_classification.html
// http://docs.opencv.org/doc/tutorials/objdetect/cascade_classifier/cascade_classifier.html

    
    Mat frame_gray;
  cvtColor( image, frame_gray, CV_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  //imshow("gray image", frame_gray);
  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
 }

bool findLargestRedObject(Mat& view, Point& location, vector<Point>& outline, int redThreshold)
{
    //allocate some images to store intermediate results
    vector<Mat> YCrCb;
    YCrCb.push_back(Mat(view.rows, view.cols, CV_8UC3));
    vector<Mat> justRed;
    justRed.push_back(Mat(view.rows, view.cols, CV_8UC1));
    vector<Mat> displayRed;
    displayRed.push_back(Mat(view.rows, view.cols, CV_8UC3));

    //Switch color spaces to YCrCb so we can detect red objects even if they are dark
    cvtColor(view, YCrCb[0], CV_BGR2YCrCb); 

    //Pull out just the red channel
    int extractRed[6]={1,0, 1, 1, 1, 2};
    mixChannels(&(YCrCb[0]), 1, &(justRed[0]), 1, extractRed, 1);

    // Threshold the red object (with the threshold from the slider)
    threshold(justRed[0], justRed[0], redThreshold, 255, CV_THRESH_BINARY);
    vector<vector<Point>> objectContours;
    vector<Vec4i> dummy;

    //Find all of the contiguous image regions
    findContours(justRed[0], objectContours, dummy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    //find the largest object
    int largestArea(-1), largestIndex(-1);
    Point largestCenter;
    for(int i=0; i<objectContours.size(); i++)
    {
        Point tempCenter;
        double tempArea;
        computeObjectAreaAndCenter(objectContours[i], tempArea, tempCenter);

        if(tempArea > largestArea)
        {
            largestArea = tempArea;
            largestIndex = i;
            largestCenter = tempCenter;
        }
    }
    location = largestCenter;
    if(largestIndex >= 0)
    {
        outline = objectContours[largestIndex];
    }

    //Construct an image for display that shows the red channel as gray
    mixChannels(&(YCrCb[0]), 1, &(displayRed[0]), 1, extractRed, 3);
    if(largestIndex >= 0)
    {
        //put a red circle around the red object
        circle(displayRed[0], largestCenter, std::min(double(view.cols)/2, sqrt(largestArea)), Scalar(0, 0, 255), 1);
    }
    imshow("Just Red", displayRed[0]);


    if(largestIndex >= 0)
    {
        return true;
    }
    else
    {
        return false;
    }

}

void onTrackbar(int value, void* data)
{
}