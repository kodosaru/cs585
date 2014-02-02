// Lab3_Part2.cpp : Defines the entry point for the console application.
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

//function to detect the largest red object in an image
bool findLargestRedObject(Mat& view, Point& location, vector<Point>& outline, int redThreshold);

//function to compute the area and center of a region bounded by a contour
void computeObjectAreaAndCenter(vector<Point>& outline, double& area, Point& center);
    
//function to draw an outline in an image
void drawOutline(Mat& image, vector<Point>& outline);

//a callback function to pass to the slider bar to threshold the red object
//this is the function that actually makes the call to find the red object
void onTrackbar(int value, void* data);

int main(int argc, char* argv[])
{
    String dataDir="/Users/donj/workspace/cs585/Lab3/Data/";
    if(argc <= 1)
    {
        cout<<"Please provide a filename of an image"<<endl;
        return 0;
    }

    //read in the image
    originalImage = imread(dataDir + argv[1]);
    Mat displayImage(originalImage.rows, originalImage.cols, CV_8UC3);

    //make a copy that we can draw on
    originalImage.copyTo(displayImage);

    //this is to store the outline of the object
    vector<Point> outline;
    int redThreshold = 190;

    namedWindow("Image Window", 1);
    createTrackbar( "Red Threshold", "Image Window", &redThreshold, 255, onTrackbar, &outline);
    onTrackbar(redThreshold, &outline);

    while(1 == 1)
    {
        originalImage.copyTo(displayImage);
        drawOutline(displayImage, outline);

        imshow("Image Window", displayImage);
        char key = waitKey(33);
        if(key == 'q')
        {
            break;
        }
        if(key == ' ')
        {
            imwrite("Lab1_Part2_result.png", displayImage);
        }
    }
    return 0;
}

void onTrackbar(int redThreshold, void* data)
{
    Point largestCenter;

    //this is how you avoid global variables in event callbacks
    vector<Point>* largestOutline = (vector<Point>*)(data);

    findLargestRedObject(originalImage, largestCenter, *largestOutline, redThreshold);
}

void drawOutline(Mat& image, vector<Point>& outline)
{
    int numPoints = outline.size()-1;
    for(int f=0; f<numPoints; f++)
    {
        line(image, outline[f], outline[f+1], Scalar(255, 0, 0), 3);
    }
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

bool findLargestRedObject(Mat& view, Point& location, vector<Point>& outline, int redThreshold)
{
    //allocate some images to store intermediate results
    vector<Mat> YCrCb; //to store the YCrCb representation
    YCrCb.push_back(Mat(view.rows, view.cols, CV_8UC3));
    vector<Mat> justRed;  //to store the red channel
    justRed.push_back(Mat(view.rows, view.cols, CV_8UC1));

    //Switch color spaces to YCrCb so we can detect red objects even if they are dark
    cvtColor(view, YCrCb[0], CV_BGR2YCrCb); 

    //Pull out just the red channel (but the red from YCrCb, not RGB)
    int extractRed[6]={1,0, 1, 1, 1, 2};
    mixChannels(&(YCrCb[0]), 1, &(justRed[0]), 1, extractRed, 1);

    // Threshold the red object (with the threshold from the slider)
    threshold(justRed[0], justRed[0], redThreshold, 255, CV_THRESH_BINARY);

    //Find all of the contiguous image regions
    //In OpenCV all of the image region representation is done with contours, 
    //rather than binary images
    vector<vector<Point>> objectContours; // memory to store the contours
    vector<Vec4i> dummy;
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
    vector<Mat> displayRed;  //to store the red channel
    displayRed.push_back(Mat(view.rows, view.cols, CV_8UC3));
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


