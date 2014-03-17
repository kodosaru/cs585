//
//  Functions.cpp
//  HW7_Part1
//
//  Created by Don Johnson on 3/16/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#include "Functions.hpp"
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;

extern bool displayMatch;
extern int match_method;

//Required: Implement the function to find the new location of the template in the image
//Arguments are: the previous location (point), the template (patch), the new image (image)
//and a search radius.
//
// You may find this OpenCV documentation helpful:
//http://docs.opencv.org/modules/imgproc/doc/object_detection.html
//http://docs.opencv.org/doc/tutorials/imgproc/histograms/template_matching/template_matching.html
void trackTemplate(Point2f& point, Mat& patch, Mat& image, Mat& displayImage, int pointNo, int searchRadius)
{
    // point - upper left corner of template
    // patch - the template/patch we seek
    // image - the new image we are search for the patch
    // searchRadius - how far from the orginal location we are searching
    
    Mat result,croppedImage;
    Rect roi(point.x-searchRadius,point.y-searchRadius,patch.cols+2*searchRadius,patch.rows+2*searchRadius);
    image(roi).copyTo(croppedImage);
    Point maxLoc;
    
    matchingMethod(croppedImage, patch, maxLoc, pointNo, 5);
    
    maxLoc.x=maxLoc.x+point.x-searchRadius;
    maxLoc.y=maxLoc.y+point.y-searchRadius;
    rectangle(displayImage, maxLoc, Point( maxLoc.x + patch.cols , maxLoc.y + patch.rows ), Scalar::all(0), 2, 8, 0 );
    //I recommend using the ROI (region of interest) functionality to crop windows.
    //Don't forget to do bounds checking so your ROIs don't go outside of the
    //boundaries of the image or you will get exceptions.
    //
    //The output of matchTemplate is with respect to the upper-left corner of the image.
    
}

void matchingMethod(Mat& img, Mat& templ, Point& maxLoc, int pointNo, int match_method)
{
    /// Source image to display
    Mat img_display, result;
    img.copyTo( img_display );
    
    /// Create the result matrix
    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;
    
    result.create( result_cols, result_rows, CV_32FC1 );
    
    /// Do the Matching and Normalize
    //cout<<"Match method: "<<match_method<<endl;
    matchTemplate( img, templ, result, match_method );
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
    
    /// Localizing the best match with minMaxLoc
    double minVal; double maxVal; Point minLoc;
    Point matchLoc;
    
    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
    
    
    /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
    if( match_method  == TM_SQDIFF || match_method == TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
    else
    { matchLoc = maxLoc; }
    
    /// Show me what you got
    rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
    rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
    
    if(displayMatch)
    {
        char sourceTitle[256];
        char resultTitle[256];
        sprintf(sourceTitle,"Source Image %d",pointNo);
        sprintf(resultTitle,"Result Image %d",pointNo);
        imshow(sourceTitle, img_display );
        imshow(resultTitle, result );
    }
    
    return;
}
