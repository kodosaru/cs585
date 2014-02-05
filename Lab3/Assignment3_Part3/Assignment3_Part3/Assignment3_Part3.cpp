// Assignment3_Part3.cpp : Defines the entry point for the console application.
//

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <string>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

Mat originalImage;

//Required: function to track the red object
void trackRedObject(Mat& image, vector<Point>& track, int redThreshold);

//Given: function to detect the largest red object in an image
bool findLargestRedObject(Mat& view, Point& location, vector<Point>& outline, int redThreshold);

//Given: Compute the area and center of a region bounded by an outline
void computeObjectAreaAndCenter(vector<Point>& outline, double& area, Point& center);

//Given: a dummy function to pass to the slider bar to threshold the red object
void onTrackbar(int value, void* data);

//Given: Draw an outline stored in a vector of points
void drawOutline(Mat& image, vector<Point>& outline);

int main(int argc, char* argv[])
{
    //using an STL vector to keep track of red object over time
    vector<Point> track;
    char directory[256], filename[256];
    int startFrame, endFrame, currentFrame;
    bool bTracking(false);
    int redThreshold(190);
    if(argc <= 3)
    {
        cout<<"Not enough arguments"<<endl;
        return 0;
    }

    strcpy(directory, argv[1]);
    startFrame = atoi(argv[2]);
    endFrame = atoi(argv[3]);
    currentFrame = startFrame;

    namedWindow("Image Window", 1);
    createTrackbar( "Red Threshold", "Image Window", &redThreshold, 255, onTrackbar, NULL);

    sprintf(filename, "%s/video_%04d.jpg", directory, currentFrame);
    originalImage = imread(filename);
    if(originalImage.empty())
    {
        cout<<"Could not read first image"<<endl;
        return 0;
    }
    onTrackbar(redThreshold, NULL);
    while(1==1)
    {
        if(bTracking && currentFrame <= endFrame)
        {
            sprintf(filename, "%s/video_%04d.jpg", directory, currentFrame);
            originalImage = imread(filename);
            if(originalImage.empty())
            {
                bTracking = false;
                continue;
            }
            trackRedObject(originalImage, track, redThreshold);
            drawOutline(originalImage, track);
            currentFrame ++;
        }
        imshow("Image Window", originalImage);
        char key = waitKey(33);
        if(key == 'q')
        {
            break;
        }
        if(key == 't')
        {
            bTracking = !bTracking;
        }
        if(key == ' ')
        {
            imwrite("Part3_result.png", originalImage);
        }
    }

	return 0;
}

void trackRedObject(Mat& view, vector<Point>& track, int redThreshold)
{
    //Required
    Point largestCenter;
    vector<Point> largestOutline;
    findLargestRedObject(view, largestCenter, largestOutline, redThreshold);
    cout<<"Center: ("<<largestCenter.x<<","<<largestCenter.y<<")"<<endl;
}

void drawOutline(Mat& image, vector<Point>& outline)
{
    int numPoints = (int)outline.size()-1;
    for(int f=0; f<numPoints; f++)
    {
        line(image, outline[f], outline[f+1], Scalar(255, 0, 0), 3);
    }
}

// Need to overload on the type of the point
void computeObjectAreaAndCenter(vector<Point>& outline, double& area, Point& center)
{
    // http://docs.opencv.org/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html
    Moments objectProperties;
    objectProperties = moments(outline, false);

    area = objectProperties.m00;
    center.x=(objectProperties.m10/area);
    center.y=(objectProperties.m01/area);
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

void onTrackbar(int redThreshold, void* data)
{
    
    Point largestCenter;
    vector<Point> largestOutline;
    findLargestRedObject(originalImage, largestCenter, largestOutline, redThreshold);
    cout<<"Center: ("<<largestCenter.x<<","<<largestCenter.y<<")"<<endl;
}
