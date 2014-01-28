//
//  Assignment2.h
//  Assignment2
//
//  Created by Donald Johnson on 1/28/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#ifndef Assignment2_Assignment2_hpp
#define Assignment2_Assignment2_hpp
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Assignment2.hpp"

using namespace std;
using namespace cv;

//Global variables are evil, but easy
extern Mat original, image, displayImage;
extern Mat maskImage;
extern Mat edges;
extern Mat gradientMagnitude, displayGradientMagnitude;
extern int outputCounter;
extern int edgeMode;
//while we are engaging in bad software practices, let's try some magic numbers
extern const int EDGEMODE_GRADIENT;
extern const int EDGEMODE_LAPLACIAN;
extern const int EDGEMODE_CANNY;

extern int smoothSlider;
extern int smoothSliderMax;
extern int threshSlider;
extern int threshSliderMax;

void fillRegionBoundedByEdges(Mat& edges, Mat& mask, int seedX, int seedY);

//Required: You must write the function to create edge maps by thresholding the gradient magnitude,
void findEdgesGradientMagnitude(Mat& image, Mat& edges, double threshold);


//Required: You must write the function to call the Canny edge detector
void findEdgesCanny(Mat& image, Mat& edges, double threshold);

//Required: You must fill in the function to use the OpenCV FloodFill command to fill the region bounded by edges
void fillRegionBoundedByEdges(Mat& edges, int seedX, int seedY);

//Required: You must write the function to refine the edge map by using morphological operators
void refineEdgesMorphological(Mat& edges, char erodeOrDilate);

//Given: Function to compute derivatives and gradient magnitude
void gradientSobel(Mat& image, Mat& dX, Mat& dY, Mat& magnitude);

//Given: the function to smooth the image is given from the lab
void smoothImage(Mat& image, double sigma);

//Given: convert a floating point image for display
bool convertGradientImageForDisplay(Mat& input, Mat& output);

//Given: display image with some region highlighted in red
bool markImageForDisplay(Mat& gray, Mat& output, Mat& mask);

//Given: read in the image and convert it to grayscale
bool readImageAndConvert(const string& filename, Mat& grayImage);

//Given: callbacks for the trackbars
void onSmoothTrackbar(int value, void* data);
void onThresholdTrackbar(int value, void* data);

#endif
