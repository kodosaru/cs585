// Lab3_Part2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <string>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

Mat originalImage;
Mat displayImage;

int main(int argc, char* argv[])
{

    if(argc <= 1)
    {
        cout<<"Please provide a filename of an image"<<endl;
        return 0;
    }

    //read in the image
    originalImage = imread(argv[1]);
    Mat grayImage;
    cvtColor(originalImage, grayImage, CV_BGR2GRAY);

    SiftFeatureDetector detector;
    vector<KeyPoint> keypoints;
    detector.detect(grayImage, keypoints);

    // Add results to image and save.
    drawKeypoints(originalImage, keypoints, displayImage, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    while(1 == 1)
    {
        imshow("Image Window", displayImage);
        char key = waitKey(33);
        if(key == 'q')
        {
            break;
        }
        if(key == ' ')
        {
        }
    }
}
