//
//  main.cpp
//  Canny
//
//  Created by Don Johnson on 1/29/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;

/// Global variables

Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int threshholdRatio = 3;
int kernel_size = 3;
String window_name = "Edge Map";

void CannyThreshold(int, void*);

int main(int argc, const char * argv[])
{
    String dataDir="/Users/donj/workspace/cs585/Lab2/Mac/Assignment2/Data/";
    
    /// Load an image
    src = imread( dataDir + argv[1] );
    
    if( !src.data )
    { return -1; }
    
    /// Create a matrix of the same type and size as src (for dst)
    dst.create( src.size(), src.type() );
    
    /// Convert the image to grayscale
    cvtColor( src, src_gray, CV_BGR2GRAY );
    
    /// Create a window
    namedWindow( window_name, CV_WINDOW_AUTOSIZE );
    
    /// Create a Trackbar for user to enter threshold
    createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
    
    /// Show the image
    CannyThreshold(0, 0);
    
    /// Wait until user exit program by pressing a key
    waitKey(0);
    
    return 0;
}

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold(int, void*)
{
    /// Reduce noise with a kernel 3x3
    blur( src_gray, detected_edges, Size(3,3) );
    
    /// Canny detector
    cout << "Low: " << lowThreshold << "  High: " << lowThreshold * threshholdRatio << endl;
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold * threshholdRatio, kernel_size );
    
    /// Using Canny's output as a mask, we display our result
    dst = Scalar::all(0);
    
    src.copyTo( dst, detected_edges);
    imshow( window_name, dst );
}

