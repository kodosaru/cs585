//
//  Methods.cpp
//  CountObjects
//
//  Created by Don Johnson on 4/8/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "CannyMethods.h"

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold(Mat& src, Mat& dst, int lowThreshold, int highThreshold, int kernelSize)
{
    Mat detectedEdges;
    
    /// Reduce noise with a kernel 3x3
    blur( src, detectedEdges, Size(3,3) );
    
    /// Canny detector
    if(DEBUG)
        cout << "Low: " << lowThreshold << "  High: " << highThreshold << endl;
    Canny(detectedEdges, detectedEdges, lowThreshold, highThreshold, kernelSize );
    
    /// Using Canny's output as a mask, we display our result
    dst = Scalar::all(0);
    
    src.copyTo(dst, detectedEdges);
    //detectedEdges.copyTo(dst);
}