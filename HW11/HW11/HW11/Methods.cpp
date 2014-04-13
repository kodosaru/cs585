//
//  Methods.cpp
//  HW11
//
//  Created by Don Johnson on 4/11/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#include "Methods.h"


void trackRedObjects(Mat& view, vector<TrackedObject>& tracks, vector<Point>& detections, int redThreshold, int areaThreshold)
{
    unsigned long nDetectionsLast = nDetections;
    if(DEBUG)
    {
        //cout<<"Called trackRedObjects with threshold "<<redThreshold<<" and area "<<areaThreshold<<endl;
        if(tracks.size()>0)
            for(int i=0; i<tracks.size(); i++)
                printf("Track: %d (%d,%d)\n",i,tracks[i].currentPosition.x,tracks[i].currentPosition.y);
    }

    vector<vector<Point> > outlines;
    vector<Point> predictions;
    if(findRedObjects(view, detections, outlines, redThreshold, areaThreshold))
    {
        if(DEBUG)
        {
            for(int z=0; z<detections.size(); z++)
            {
                printf("detection: No: %d (%d,%d)\n",z,detections[z].x,detections[z].y);
            }
        }
        
        // initialze tracks if we don't have any already
        nDetections=detections.size();
        //if(DEBUG)
            //printf("Detections: %ld and last detections: %ld\n",nDetections,nDetectionsLast);
        if(tracks.empty() || (nDetections != nDetectionsLast))
        {
            //tracks.clear();
            for(int z=0; z<detections.size(); z++)
            {
                tracks.push_back(TrackedObject(detections[z]));
                if(DEBUG && (nDetections != tracks.size()))
                    printf("ERROR: No. of detections and tracks is different");
                
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
            if(DEBUG)
                cout<<"No detections"<<endl;
            return;
        }
        
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
        Mat cost(tracks.size(), detections.size(), CV_32FC1);
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
