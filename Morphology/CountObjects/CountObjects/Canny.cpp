//
//  main.cpp
//
//  Created by Don Johnson on 1/29/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "CannyMethods.h"

using namespace cv;
using namespace std;

/// Global variables

Mat srcOriginal, srcOriginalGray;
Mat dstCanny;

int lowThreshold;
int const maxLowThreshold = 100;
int threshholdRatio = 3;
int kernel_size = 3;
String originalWindow = "Original Image";
String cannyWindow = "Edge Map";

int Canny(int argc, const char * argv[])
{
    String dataDir="/Users/donj/workspace/cs585/Morphology/Data/";
    
    /// Load an image
    srcOriginal = imread( dataDir + argv[1] );
    resize(srcOriginal,srcOriginal,Size(srcOriginal.cols/3,srcOriginal.rows/3));
    
    if( !srcOriginal.data )
    { return -1; }

    
    /// Create a matrix of the same type and size as src (for dst)
    dstCanny.create( srcOriginal.size(), srcOriginal.type() );
    
    /// Convert the image to grayscale
    cvtColor( srcOriginal, srcOriginalGray, CV_BGR2GRAY );
    
    /// Create a window
    namedWindow( originalWindow, CV_WINDOW_AUTOSIZE );
    namedWindow( cannyWindow, CV_WINDOW_AUTOSIZE );
    
    /// Create a Trackbar for user to enter threshold
    createTrackbar( "Min Threshold:", cannyWindow, &lowThreshold, maxLowThreshold, callbackCanny);
    
    /// Show the image
    callbackCanny(lowThreshold, 0);
    
    /// Wait until user exit program by pressing a key
    while(1)
    {
        char c = (char)waitKey(0);
        switch( c )
        {
            case 'w':
                imwrite(dataDir+"temp1.png",  dstCanny);
                break;
            case 'q':
            case 27:
                return(0);
        }
    }
}

void callbackCanny(int, void*)
{
    CannyThreshold(srcOriginalGray, dstCanny, lowThreshold, lowThreshold*threshholdRatio, 3);
    imshow(cannyWindow,dstCanny);
}
