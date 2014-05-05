// OpenCVWebcam.cpp : Defines the entry point for the console application.
//
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/video/tracking.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <math.h>

using namespace cv;
using namespace std;

//Documentation for OpenCV's Kalman Filter
//http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html#kalmanfilter

//A class to help keep track of the history of tracked objects
class TrackedObject
{
public:
    TrackedObject(Point& firstMeasurement):estimator(4,2,0)
    {
        // Required: Write down the matrix that is being constructed here and explain what it means
        estimator.transitionMatrix.at<float>(0,0) = 1;
        estimator.transitionMatrix.at<float>(0,2) = 1;
        estimator.transitionMatrix.at<float>(1,1) = 1;
        estimator.transitionMatrix.at<float>(1,3) = 1;
        estimator.transitionMatrix.at<float>(2,2) = 1;
        estimator.transitionMatrix.at<float>(3,3) = 1;
        
        setIdentity(estimator.measurementMatrix);
        setIdentity(estimator.processNoiseCov);
        setIdentity(estimator.measurementNoiseCov);
        setIdentity(estimator.errorCovPost);
        
        estimator.statePost.at<float>(0) = firstMeasurement.x;
        estimator.statePost.at<float>(1) = firstMeasurement.y;
        
        currentPosition = firstMeasurement;
    }
    
    Point getPrediction()
    {
        Point predictedPoint;
        Mat prediction = estimator.predict();
        predictedPoint.x = prediction.at<float>(0);
        predictedPoint.y = prediction.at<float>(1);
        return predictedPoint;
    }
    
    void update(Point& measuredPoint)
    {
        Mat measurement = Mat::zeros(2, 1, CV_32F);
        measurement.at<float>(0) = measuredPoint.x;
        measurement.at<float>(1) = measuredPoint.y;
        estimator.correct(measurement);
        
        currentPosition.x = estimator.statePost.at<float>(0);
        currentPosition.y = estimator.statePost.at<float>(1);
        
        //appends a copy of the currentPosition, not a reference / pointer to it
        history.push_back(currentPosition);
    }
    
    Point currentPosition;
    vector<Point> history;
    KalmanFilter estimator;
    Scalar color;
};


//You have work to do in this function
//function to track the red object
void trackRedObjects(Mat& image, vector<TrackedObject>& tracks, int redThreshold, int areaThreshold);

//functions to detect the largest red object in an image
bool findRedObjects(Mat& view, vector<Point>& locations, vector<vector<Point> >& outlines, int redThreshold, int areaThreshold);

//a dummy function to pass to the slider bar to threshold the red object
void onTrackbar(int value, void* data);

// Compute the area and center of a region bounded by an outline
void computeObjectAreaAndCenter(vector<Point>& outline, double& area, Point& center);

//Draw an outline stored in a vector of points
void drawLine(Mat& image, vector<Point>& outline, Scalar color);

//Compute distance between two points
double computeDistance(Point p1, Point p2);


struct PATH
{
    int track0Detection;
    int track1Detection;
    int track2Detection;
    float totalCost;
};

// modified by donj
int sumCostPerPath(Mat& cost, vector<TrackedObject>& tracks, vector<Point>& detections, struct PATH path)
{
    const int nObjects=3;
    if(tracks.size() != nObjects || detections.size() != nObjects)
    {
        printf("Tracks: %ld and detections: %ld\n",tracks.size(),detections.size());
        printf("Using simple data association\n");
        return 1;
    }
    float ftemp=0.0;
    path.totalCost=FLT_MAX;
    
    for(int i=0;i<nObjects;i++)
        for(int j=0;j<nObjects;j++)
            for(int k=0;k<nObjects;k++)
            {
                if(i==j || i==k || j==k)
                {
                    path.totalCost=FLT_MAX;
                    printf("Path Conflict (%d,%d,%d)\n",path.track0Detection=i,path.track1Detection=j,path.track2Detection=k);
                }
                else
                {
                    ftemp=cost.at<float>(0,i);
                    ftemp+=cost.at<float>(1,j);
                    ftemp+=cost.at<float>(2,k);
                    if(ftemp<path.totalCost)
                    {
                        //Create a vector of the object,detection paring with the smallest path cost
                        path.track0Detection=i;
                        path.track1Detection=j;
                        path.track2Detection=k;
                        path.totalCost=ftemp;
                        printf("Path Cost (%d,%d,%d): %f\n",path.track0Detection=i,path.track1Detection=j,path.track2Detection=k,path.totalCost);
                    }
                }
            }
    
    return 0;
}

// modified by donj
void dataAssociationNew(vector<TrackedObject>& tracks, vector<Point>& detections)
{
    vector<Point> predictions;
    
    // Collect the predicted locations of our tracked objects
    for(int t=0; t<tracks.size(); t++)
    {
        Point prediction=tracks[t].getPrediction();
        predictions.push_back(prediction);
        //if(DEBUG)
        //{
        //    printf("predicted point: track %d (%d,%d)\n",t,prediction.x,prediction.y);
        //}
        
    }
    
    // compute the cost matrix to use to decide which detections should go to which tracks
    Mat cost((int)tracks.size(), (int)detections.size(), CV_32FC1);
    vector<int> bestMatch;
    vector<int> bestScore;
    for(int t=0; t<tracks.size(); t++)
    {
        for(int z=0; z<detections.size(); z++)
        {
            cost.at<float>(t,z) = computeDistance(predictions[t], detections[z]);
        }
    }
    
    
    // Given: Naive data association: just give every track its closest measurement.
    //
    // Required: Observe some bad things that happen when this style of data association is used
    // Required: Implement a better data association algorithm by enumerating the total cost of
    //           all possible assignments and choosing the best for up to 3 measurements and 3 tracks
    // Required: Add logic so that any detection not assigned to a track results in the creation of a new track (up to 3 tracks)
    // Required: Add logic to allow tracks to terminate if not associated with a sufficiently close detection
    
    PATH path;
    sumCostPerPath(cost, tracks, detections, path);
    
    for(int t=0; t<tracks.size(); t++)
    {
        bestMatch.push_back(-1);
        bestScore.push_back(10000);
        for(int z=0; z<detections.size(); z++)
        {
            if(cost.at<float>(t,z) < bestScore[t])
            {
                bestMatch[t]=z;
                bestScore[t] = cost.at<float>(t,z);
            }
        }
        
        //update the track with the nearest measurement
        tracks[t].update(detections[bestMatch[t]]);
    }
}

// modified by donj
inline void findFarthestTrack(vector<TrackedObject>& tracks, vector<Point>& detections, int& maxNdx, float& maxDist)
{
    maxDist=-MAXFLOAT;
    float dist;
    maxNdx=-1;
    for(int i=0;i<tracks.size();i++)
    {
        for(int j=0;j<detections.size();j++)
        {
            dist = sqrt(pow(tracks[i].currentPosition.x - detections[j].x,2) + pow(tracks[i].currentPosition.y - detections[j].y,2));
            if(dist>maxDist)
            {
                maxDist=dist;
                maxNdx=i;
            }
        }
    }
}

// modified by donj
inline void removeDuplicateTracks(vector<TrackedObject>& tracks)
{
    int dupNdx = -1;
    float dist;
    for(int i=0;i<tracks.size();i++)
    {
        for(int j=0;j<tracks.size();j++)
        {
            if(i != j)
            {
                if(tracks[i].currentPosition.x == tracks[j].currentPosition.x && tracks[i].currentPosition.y == tracks[j].currentPosition.y)
                {
                    dupNdx = i;
                }
                else
                {
                    dist = sqrt(pow(tracks[i].currentPosition.x - tracks[j].currentPosition.x,2) + pow(tracks[i].currentPosition.y - tracks[j].currentPosition.y,2));
                    if(dist < 10)
                        dupNdx = i;
                }
            }
        }
    }
    if(dupNdx > -1)
        tracks.erase(tracks.begin()+dupNdx);
}

int main(int argc, char* argv[])
{
    string dataDir="/Users/donj/workspace/cs585/HW11/Data/";
    VideoCapture capture;
    char filename[256];
    capture.open(0);
    if(!capture.isOpened())
    {
        int error = -1;
        return 1;
    }
    
    //for tracking the balls
    //int redThreshold=120;
    //int areaThreshold = 300;
    int redThreshold=170;
    int areaThreshold = 200;
    bool bTracking = false;
    vector<TrackedObject> tracks;
    
    //for recording frames of video and results
    bool bRecordVideo = false;
    bool bRecordResults = false;
    int frameNumber=0;
    
    //create the window with a trackbar for the slider
    namedWindow( "Camera View", 1 );
    createTrackbar( "Red Threshold", "Camera View", &redThreshold, 255, onTrackbar );
    createTrackbar( "Area Threshold", "Camera View", &areaThreshold, 1000, onTrackbar );
    
    Mat view, view0;
    capture.read(view0);
    view0.copyTo(view);
    
    bool blink = false;
    
    while( capture.isOpened() )
    {
        capture.read(view0);
        view0.copyTo(view);
        string path=dataDir+"video/video_%04d.jpg";
        if(bRecordVideo)
        {
            sprintf(filename, path.c_str(), frameNumber);
            imwrite(filename, view);
        }
        
        if(bTracking)
        {
            //We are currently tracking the red object, so draw the track so the user can see it
            trackRedObjects(view, tracks, redThreshold, areaThreshold);
            //draw the tracks
            cout<<"No. tracks: "<<tracks.size()<<endl;
            for(int t=0; t<tracks.size(); t++)
            {
                drawLine(view, tracks[t].history, tracks[t].color);
            }
        }
        // modified by donj
        path=dataDir+"results/results_%04d.jpg";
        if(bRecordResults && (((frameNumber % 10) == 0) || frameNumber == 1))
        {
            sprintf(filename, path.c_str(), frameNumber);
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
                tracks.clear();
            }
        }
        if(key == 'v')
        {
            bRecordVideo = !bRecordVideo;
        }
        if(key == 'r')
        {
            frameNumber=0;
            bRecordResults = true;
        }
        if(key == 's')
        {
            bRecordResults = false;
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

void trackRedObjects(Mat& view, vector<TrackedObject>& tracks, int redThreshold, int areaThreshold)
{
    int maxNdx;
    float maxDist;
    vector<Point> detections;
    for(int i=0; i<tracks.size(); i++)
    {
        printf("Track: %d (%d,%d)\n",i,tracks[i].currentPosition.x,tracks[i].currentPosition.y);
    }

    vector<vector<Point> > outlines;
    vector<Point> predictions;
    if(findRedObjects(view, detections, outlines, redThreshold, areaThreshold))
    {
        for(int i=0; i<detections.size(); i++)
        {
            printf("Detection: %d (%d,%d)\n",i,detections[i].x,detections[i].y);
        }

        // initialze tracks if we don't have any already
        removeDuplicateTracks(tracks);
        if(detections.size() != tracks.size())
        {
            for(int z=0; z<detections.size(); z++)
            {
                // modified by donj
                if(detections.size()>tracks.size())
                {
                    // Add new tracking object to go with new detection
                   tracks.push_back(TrackedObject(detections[z]));
                }
                else if(detections.size()<tracks.size())
                {
                    findFarthestTrack(tracks,detections,maxNdx,maxDist);
                    printf("Min ndx: %d dist: %0.2f\n", maxNdx,maxDist);
                    // Delete track that is farthest from any existing object;
                    tracks.erase(tracks.begin()+maxNdx);
                }
                
                //Assign different colors so we can see the track identity
                switch(z%3)
                {
                    case 0:
                        tracks.back().color = Scalar(255, 0, 0);
                        break;
                    case 1:
                        tracks.back().color = Scalar(0, 255, 0);
                        break;
                    case 2:
                        tracks.back().color = Scalar(0, 255, 255);
                        break;
                }
            }
            return;
        }
        
        // Can't do anything with no detections
        if(detections.empty())
        {
            return;
        }
        
        // Collect the predicted locations of our tracked objects
        for(int t=0; t<tracks.size(); t++)
        {
            predictions.push_back(tracks[t].getPrediction());
        }
        
        // Associate data and draw tracks
        //dataAssociationNew(tracks, detections);

        
        // modified by donj
        // compute the cost matrix to use to decide which detections should go to which tracks
        int dims[] = {(int)tracks.size(),(int)detections.size(),2};
        Mat cost(3, dims, CV_32FC1);
        //Mat cost(tracks.size(), detections.size(), CV_32FC1);
        vector<int> bestMatch;
        vector<int> bestScore;
        for(int t=0; t<tracks.size(); t++)
        {
            for(int z=0; z<detections.size(); z++)
            {
                cost.at<float>(t,z,0) = computeDistance(predictions[t], detections[z]);
                cost.at<float>(t,z,1) = 0;

            }
        }
        
        // Given: Naive data association: just give every track its closest measurement.
        //
        // Required: Observe some bad things that happen when this style of data association is used
        // Required: Implement a better data association algorithm by enumerating the total cost of
        //           all possible assignments and choosing the best for up to 3 measurements and 3 tracks
        // Required: Add logic so that any detection not assigned to a track results in the creation of a new track (up to 3 tracks)
        // Required: Add logic to allow tracks to terminate if not associated with a sufficiently close detection
        for(int t=0; t<tracks.size(); t++)
        {
            bestMatch.push_back(-1);
            bestScore.push_back(10000);
            for(int z=0; z<detections.size(); z++)
            {
                if(cost.at<float>(t,z,0) < bestScore[t] && cost.at<float>(t,z,1) != 1)
                {
                    bestMatch[t]=z;
                    bestScore[t] = cost.at<float>(t,z,0);
                    //printf("Best score track %d detect %d: %d\n",t,z,bestScore[t]);
                    // Mark cost as used
                    cost.at<float>(t,z,1) = 1;
                }
            }
            
            //update the track with the nearest measurement
            tracks[t].update(detections[bestMatch[t]]);
        }
    }
}

void drawLine(Mat& image, vector<Point>& outline, Scalar color)
{
    int numPoints = outline.size()-1;
    for(int f=0; f<numPoints; f++)
    {
        line(image, outline[f], outline[f+1], color, 3);
    }
}

double computeDistance(Point p1, Point p2)
{
    double x = p1.x - p2.x;
    double y = p1.y - p2.y;
    return sqrt(x*x+y*y);
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

bool findRedObjects(Mat& view, vector<Point>& locations, vector<vector<Point> >& outlines, int redThreshold, int areaThreshold)
{
    locations.clear();
    outlines.clear();
    
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
    
    //find the sufficiently large objects
    for(int i=0; i<objectContours.size(); i++)
    {
        Point tempCenter;
        double tempArea;
        computeObjectAreaAndCenter(objectContours[i], tempArea, tempCenter);
        
        if(tempArea > areaThreshold)
        {
            locations.push_back(tempCenter);
            outlines.push_back(objectContours[i]);
        }
    }
    
    //Construct an image for display that shows the red channel as gray
    mixChannels(&(YCrCb[0]), 1, &(displayRed[0]), 1, extractRed, 3);
    for(int i=0; i<locations.size(); i++)
    {
        drawLine(displayRed[0], outlines[i], Scalar(0, 0, 255));
    }
    
    imshow("Just Red", displayRed[0]);
    
    
    if(locations.size() >= 0)
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
