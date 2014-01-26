// CS585 Assignment 2
// Edge Detection in OpenCV
//
//
// Copyright 2014 Diane H. Theriault
//
//
#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

//Global variables are evil, but easy
Mat original, image, displayImage;
Mat maskImage;
Mat edges;
Mat gradientMagnitude, displayGradientMagnitude;
int outputCounter;
int edgeMode; 
//while we are engaging in bad software practices, let's try some magic numbers
const int EDGEMODE_GRADIENT=0;
const int EDGEMODE_LAPLACIAN=1;
const int EDGEMODE_CANNY=2;

int smoothSlider;
int smoothSliderMax = 20;
int threshSlider;
int threshSliderMax = 256;

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

//Given: the callback for the click
void onClickCallback( int event, int x, int y, int q, void* data);

//Given: callbacks for the trackbars
void onSmoothTrackbar(int value, void* data);
void onThresholdTrackbar(int value, void* data);

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout<<"Usage: Lab2 imageName"<<endl;
        return 0;
    }
    outputCounter=1;
    edgeMode = EDGEMODE_GRADIENT;

    //load in the image and convert it to gray scale
    readImageAndConvert(argv[1], original);
    if(original.empty())
    {
        cout<<"Unable to open the image file: "<<argv[1]<<endl;
        return 0;
    }
    original.copyTo(image);

    //initialize the versions of the image that we will use for displaying some things
    displayImage.create(image.rows, image.cols, CV_8UC3);
    maskImage=Mat::zeros(image.rows, image.cols, CV_8UC1);
    markImageForDisplay(image, displayImage, maskImage);

    Mat dX, dY;
    gradientMagnitude.create(image.rows, image.cols, CV_32FC1);
    displayGradientMagnitude.create(image.rows, image.cols, CV_8UC1);
    gradientSobel(image, dX, dY, gradientMagnitude);
    convertGradientImageForDisplay(gradientMagnitude, displayGradientMagnitude);

    edges.create(image.rows, image.cols, CV_8UC1);

    // Create a new windows
    namedWindow("Control Window",0);
    namedWindow( "Image View", 1);
    namedWindow( "Edges", 1);
    namedWindow( "Gradient Magnitude", 1);

    //attach a mouse click callback to the window
    setMouseCallback("Image View", onClickCallback, NULL);
    createTrackbar( "Smoothing", "Control Window", &smoothSlider, smoothSliderMax, onSmoothTrackbar );
    createTrackbar( "Threshold", "Control Window", &threshSlider, threshSliderMax, onThresholdTrackbar );

    //display the images and wait for 33 milliseconds in a loop, to 
    //allow us to refresh the displayed image when we click at locations in the image
    while(1)
    {
       imshow("Image View", displayImage);
       //imshow("Mask View", maskImage);
       imshow("Gradient Magnitude", displayGradientMagnitude);
       imshow("Edges", edges);
       char key=waitKey(33);
       if(key == 'Q' || key == 'q')
       {
           break;
       }
       if(key == 'S' || key == 's')
       {
           char filename1[512];
           char filename2[512];
           sprintf_s(filename1, "filledRegion_%03d.png", outputCounter);
           imwrite(filename1, displayImage);
           sprintf_s(filename2, "edges_%03d.png", outputCounter);
           imwrite(filename2, edges);

           cout<<"Images Saved: "<<filename1<<" "<<filename2<<endl;
           outputCounter++;
       }
       if(key == 'g')
       {
           edgeMode = EDGEMODE_GRADIENT;
           onThresholdTrackbar(threshSlider, NULL);
       }
       if(key == 'c')
       {
           edgeMode = EDGEMODE_CANNY;
           onThresholdTrackbar(threshSlider, NULL);
       }
       if(key == 'e' || key == 'd')
       {
            refineEdgesMorphological(edges, key);
       }

    }

    return 0;
}

void fillRegionBoundedByEdges(Mat& edges, Mat& mask, int seedX, int seedY)
{
   edges.copyTo(mask);
   int channels = mask.channels();
   //http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#floodfill
}

void refineEdgesMorphological(Mat& edges, char erodeOrDilate)
{
    //http://docs.opencv.org/doc/tutorials/imgproc/erosion_dilatation/erosion_dilatation.html

    if(erodeOrDilate == 'e')
    {
    }
    else if(erodeOrDilate == 'd')
    {
    }
}


void findEdgesCanny(Mat& image, Mat& edges, double thresh)
{
    //http://docs.opencv.org/doc/tutorials/imgproc/imgtrans/canny_detector/canny_detector.html
}


void findEdgesGradientMagnitude(Mat& image, Mat& edges, double thresh)
{
}


void gradientSobel(Mat& image, Mat& dX, Mat& dY, Mat& magnitude)
{
    //http://docs.opencv.org/modules/imgproc/doc/filtering.html?highlight=sobel#sobel
    //calculate the gradients with OpenCV Sobel() function
    Sobel( image, dX, CV_32F, 1, 0, 3, 1, 0, BORDER_DEFAULT );
    Sobel( image, dY, CV_32F, 0, 1, 3, 1, 0, BORDER_DEFAULT );

    //to compute gradient magnitude:
    // http://docs.opencv.org/modules/core/doc/operations_on_arrays.html?highlight=multiply#multiply
    // http://docs.opencv.org/modules/core/doc/operations_on_arrays.html?highlight=add#add
    // http://docs.opencv.org/modules/core/doc/operations_on_arrays.html?highlight=sqrt#sqrt

    Mat temp(dX.rows, dY.cols, CV_32F);
    multiply(dX, dX, temp);
    temp.copyTo(magnitude);
    multiply(dY, dY, temp);
    add(temp, magnitude, magnitude);
    sqrt(magnitude, magnitude);
}


void smoothImage(Mat& image, double sigma)
{
    if(sigma <= 0.0)
    {
        return;
    }
    //smooth the image 
    // This is another example of a convolution / filtering operation, this time with a 
    // Gaussian kernel. You could also use all ones to get the mean of the pixels in the image
    GaussianBlur(image, image, Size(0,0), sigma, sigma, BORDER_DEFAULT);
}

bool convertGradientImageForDisplay(Mat& input, Mat& output)
{
    if(input.empty())
    {
        return false;
    }
    if(output.empty() || input.rows != output.rows || input.cols != output.cols)
    {
        return false;
    }

    Mat temp; //make a copy so we don't change the input image
    input.copyTo(temp);
    //convert to range 0 - 255
    double minVal, maxVal;
    minMaxLoc(temp, &minVal, &maxVal);

    //keep zero centered at 128
    if(minVal < 0)
    {
        maxVal = max(abs(minVal), abs(maxVal));
        minVal = -maxVal;
    }

    //scale image into range [0 255] and convert from float to unsigned char
    temp = (temp-minVal) / (maxVal-minVal)*255;
    temp.convertTo(output, CV_8UC1);
    return true;
}

bool markImageForDisplay(Mat& gray, Mat& output, Mat& mask)
{
    //duplicate gray image into three channels and place one red pixel
    vector<Mat> channels;
    for(int i=0; i<3; i++)
    {
        channels.push_back(gray);
    }
    merge(channels, output);

    //anywhere that is marked in the mask image, suppress the green and blue 
    //channels so that the region will be highlighted red
    int numchannels = mask.channels();
    for(int row=0; row<output.rows; row++)
    {
        unsigned char* maskPtr = mask.ptr<unsigned char>(row);
        unsigned char* imgPtr = output.ptr<unsigned char>(row);
        for(int col=0; col<output.cols; col++)
        {
            if(maskPtr[col]==255)
            {
                imgPtr[col*3]=0;
                imgPtr[col*3+1]=0;
            }
        }
    }
    return true;
}

bool readImageAndConvert(const string& filename, Mat& grayImage)
{
    //read image from file and convert to gray scale
    Mat image;
    image = imread(filename);
    if(image.empty())
    {
        return false;
    }
    cvtColor( image, grayImage, CV_RGB2GRAY );
    return !grayImage.empty();
}

static void onClickCallback( int event, int x, int y, int q, void* data)
{
    if( event != EVENT_LBUTTONDOWN )
        return;

    //Flood fill region bounded by edges
    fillRegionBoundedByEdges(edges, maskImage, x, y);

    markImageForDisplay(image, displayImage, maskImage);
}

void onSmoothTrackbar(int value, void* data)
{
    original.copyTo(image);
    smoothImage(image, value);
    //gradientSobel(image, dX, dY);
    markImageForDisplay(image, displayImage, maskImage);

    Mat dX, dY;
    gradientSobel(image, dX, dY, gradientMagnitude);
    convertGradientImageForDisplay(gradientMagnitude, displayGradientMagnitude);

    onThresholdTrackbar(threshSlider, NULL);
}

void onThresholdTrackbar(int value, void* data)
{
    //re-initialize the edge map and mask?
    edges = Mat::zeros(edges.rows, edges.cols, CV_8U);
    if(edgeMode == EDGEMODE_GRADIENT)
    {
      findEdgesGradientMagnitude(image, edges, threshSlider);
    }
    if(edgeMode == EDGEMODE_CANNY)
    {
        findEdgesCanny(image, edges, threshSlider);
        edges = edges/2;
    }
    edges.copyTo(maskImage);
    markImageForDisplay(image, displayImage, maskImage);
}
