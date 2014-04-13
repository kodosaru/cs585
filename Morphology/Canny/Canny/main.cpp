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

Mat src, src_gray, src2, src2_gray;
Mat dst, detected_edges, dst2, detected2_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int threshholdRatio = 3;
int kernel_size = 3;
String window_name = "Edge Map";

int edgeThresh2 = 1;
int lowThreshold2;
int const max_lowThreshold2 = 100;
int threshholdRatio2 = 3;
int kernel_size2 = 3;
String window_name2 = "Edge Map Reflective BG";


void CannyThreshold(int, void*);
void CannyThreshold2(int, void*);

int main(int argc, const char * argv[])
{
    String dataDir="/Users/donj/workspace/cs585/Morphology/Data/";
    
    /// Load an image
    src = imread( dataDir + argv[1] );
    resize(src,src,Size(src.cols/3,src.rows/3));
    src2 = imread( dataDir + argv[2] );
    resize(src2,src2,Size(src2.cols/2,src2.rows/2));
    
    if( !src.data )
    { return -1; }
    if( !src2.data )
    { return -1; }
    
    /// Create a matrix of the same type and size as src (for dst)
    dst.create( src.size(), src.type() );
    dst2.create( src2.size(), src2.type() );
    
    /// Convert the image to grayscale
    cvtColor( src, src_gray, CV_BGR2GRAY );
    cvtColor( src2, src2_gray, CV_BGR2GRAY );
    
    /// Create a window
    namedWindow( window_name, CV_WINDOW_AUTOSIZE );
    namedWindow( window_name2, CV_WINDOW_AUTOSIZE );
    
    /// Create a Trackbar for user to enter threshold
    createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
    createTrackbar( "Min Threshold:", window_name2, &lowThreshold2, max_lowThreshold2, CannyThreshold2 );
    
    /// Show the image
    CannyThreshold(0, 0);
    CannyThreshold2(0, 0);
    
    /// Wait until user exit program by pressing a key
    while(1)
    {
        char c = (char)waitKey(0);
        switch( c )
        {
            case 'w':
                //imwrite(dataDir+"nonreflective_bg.png",  dst);
                //imwrite(dataDir+"reflective_bg.png",  dst2);
                imwrite(dataDir+"edge1.png",  dst);
                imwrite(dataDir+"edge2.png",  dst2);
                
                break;
            case 'q':
            case 27:
                return(0);
        }
    }
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

void CannyThreshold2(int, void*)
{
    /// Reduce noise with a kernel 3x3
    blur( src2_gray, detected2_edges, Size(3,3) );
    
    /// Canny detector
    cout << "Low: " << lowThreshold2 << "  High: " << lowThreshold2 * threshholdRatio2 << endl;
    Canny( detected2_edges, detected2_edges, lowThreshold2, lowThreshold2 * threshholdRatio2, kernel_size2 );
    
    /// Using Canny's output as a mask, we display our result
    dst2 = Scalar::all(0);
    
    src2.copyTo( dst2, detected2_edges);
    imshow( window_name2, dst2 );
}
