//
//  Functions.cpp
//  HW8
//
//  Created by Don Johnson on 3/26/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#include "Functions.h"

//Modified by Don Johnson
void opticalFlowMagnitudeAngle(const Mat& flow, Mat& magnitude, Mat& angle, int step)
{
    if(magnitude.rows != flow.rows || magnitude.cols != flow.cols)
    {
        magnitude.create(flow.rows, flow.cols, CV_32FC1);
    }
    if(angle.rows != flow.rows || angle.cols != flow.cols)
    {
        angle.create(flow.rows, flow.cols, CV_32FC1);
    }
    
    double rad_to_degree_factor=180.0/M_PI;
    //Here, use a double for loop to calculate the magnitude and orientation of the flow.
    for(int y = 0; y < flow.rows; y += step)
        for(int x = 0; x < flow.cols; x += step)
        {
            //Get the flow vector as a Point2f object
            const Point2f& fxy = flow.at<Point2f>(y, x);
            
            // Draw a line from the image point using the flow vector
            magnitude.at<float>(y, x)=sqrt(pow(fxy.x,2) + pow(fxy.y,2));
            angle.at<float>(y, x)=rad_to_degree_factor * atan2(fxy.y,fxy.x);
        }
    
}

//Modified by Don Johnson
void recordFrames(Mat& view, Mat& magnitude, Mat& angle, vector<Mat>& view_record, vector<Mat>& magnitude_record, vector<Mat>& angle_record, bool& bRecording, int frameNumber, int nFrame[], int& fcount)
{
    if(frameNumber % N_SKIP_FRAMES == 0)
    {
        if(fcount < N_SAVE_FRAMES)
        {
            nFrame[fcount]=frameNumber;
            cout<<"Saving frame "<<nFrame[fcount]<<endl;
            view.copyTo(view_record[fcount]);
            magnitude.copyTo(magnitude_record[fcount]);
            angle.copyTo(angle_record[fcount]);
            fcount++;
        }
        else
        {
            bRecording=false;
        }
    }
}

//Modified by Don Johnson
void writeFrames(vector<Mat>& view_record, vector<Mat>& magnitude_record, vector<Mat>& angle_record, char* directory,int nFrame[])
{
    char filename[256];
    
    for(int i;i<N_SAVE_FRAMES;i++)
    {
        sprintf(filename, "%s/video_%04d.jpg", directory, nFrame[i]);
        imwrite(filename, view_record[i]);
        sprintf(filename, "%s/magnitude_%04d.jpg", directory, nFrame[i]);
        imwrite(filename, magnitude_record[i]);
        sprintf(filename, "%s/angle_%04d.jpg", directory, nFrame[i]);
        imwrite(filename, angle_record[i]);
    }
}

// copied from OpenCV sample fback.cpp
void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step, double, const Scalar& color)
{
    // Optical flow is stored as two-channel floating point array in a Mat
    // of type CV_32FC2
    for(int y = 0; y < cflowmap.rows; y += step)
        for(int x = 0; x < cflowmap.cols; x += step)
        {
            //Get the flow vector as a Point2f object
            const Point2f& fxy = flow.at<Point2f>(y, x);
            
            // Draw a line from the image point using the flow vector
            line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),
                 color);
            //circle(cflowmap, Point(x,y), 2, color, -1);
        }
}

//Given: Function to visualize the magnitude and orientation of the flow field
//The magnitude needs to be normalized for visualization. On my test video, the flow was not more than 10.
//You may want to adjust the maximum flow or computer it automatically
void visualizeFlow(const Mat& magnitude, const Mat& angle, Mat& magnitude8UC1, Mat& angle8UC3)
{
    if(magnitude8UC1.rows != magnitude.rows || magnitude8UC1.cols != magnitude.cols || magnitude8UC1.type() != CV_8UC3)
    {
        magnitude8UC1.create(magnitude.rows, magnitude.cols, CV_8UC1);
    }
    if(angle8UC3.rows != angle.rows || angle8UC3.cols != angle.cols || angle8UC3.type() != CV_8UC3)
    {
        angle8UC3.create(angle.rows, angle.cols, CV_8UC3);
    }
    
    double minMag, maxMag;
    //minMaxLoc(magnitude, &minMag, &maxMag); //to automatically compute the maximum
    minMag = 0;
    maxMag = 10; // your mileage my vary
    
    Mat tempMagnitude;
    tempMagnitude = (magnitude - minMag)/(maxMag-minMag)*255;
    tempMagnitude.convertTo(magnitude8UC1, CV_8UC1);
    
    //Going to create a rainbow image where the hue correspondes to the angle
    for(int y=0; y<angle.rows; y++)
    {
        unsigned char* rowPtr = angle8UC3.ptr<unsigned char>(y);
        for(int x=0; x<angle.cols; x++)
        {
            int index = x*3;
            rowPtr[index] = (angle.at<float>(y,x)+180)/2.0;
            rowPtr[index+1] = 128; //don't use full saturation or you'll go blind.
            rowPtr[index+2] = 196;
        }
    }
    cvtColor(angle8UC3, angle8UC3, CV_HSV2BGR);
}

