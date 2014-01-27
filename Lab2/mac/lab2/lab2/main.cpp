//
//  main.cpp
//  lab2
//
//  Created by Donald Johnson on 1/26/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//


// CS585 Lab 2
// Gradients in OpenCV
//
// This program reads an image from a file, converts it to grayscale,
// then calculates the derivatives in the X and Y directions.
// The image and the derivative images are displayed in separate window.
// If a user clicks in the image, the value of the x and y components of
// the gradient at the clicked location is printed to the terminal,
// and a red box is displayed overlayed on the image.
//
// Copyright 2014 Diane H. Theriault
//
//
//#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

//Global variables are evil, but easy
Mat original, image, displayImage;
Mat maskImage;
Mat dX, dY, gradientMagnitude, laplacian;
Mat displayDX, displayDY, displayMagnitude, displayLaplacian;
int outputCounter;

int smoothSlider;
int smoothSliderMax = 10;

//what not to do
void gradient_FiniteDifferences_v1(Mat& image, Mat& dX, Mat& dY);
void gradient_FiniteDifferences_v2(Mat& image, Mat& dX, Mat& dY);
void gradient_Convolution(Mat& image, Mat& dX, Mat& dY);
void convolutionByHand(Mat& image, Mat& coefficients, Mat& result);


//the better way to do it
void gradient_OpenCV_v1(Mat& image, Mat& dX, Mat& dY);
void gradient_OpenCV_v2(Mat& image, Mat& dX, Mat& dY);

//smooth image
void smoothImage(Mat& image, double sigma);

//compute derivatives and gradient magnitude
void gradientSobel(Mat& image, Mat& dX, Mat& dY, Mat& magnitude);

//convert gradient image for display by scaling into the range [0 255]
bool convertGradientImageForDisplay(Mat& input, Mat& output);

//duplicate the grayscale image 3 times to get a 3 channel image so that we
//can then manipulate parts of it to paint them red
bool markImageForDisplay(Mat& gray, Mat& output, Mat& mask);

//read in the image and convert it to grayscale
bool readImageAndConvert(const string& filename, Mat& grayImage);

//the callback for the click
static void onClickCallback( int event, int x, int y, int q, void* data);

//a function to smooth the image, based on the smoothing factor chosen with the trackbar
void onTrackbar(int value, void* data);

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout<<"Usage: Lab2 imageName"<<endl;
        return 0;
    }
    outputCounter=1;
    
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
    
    //allocate objects for holding derivatives and gradients
    dX.create(image.rows, image.cols, CV_32F);
    dY.create(image.rows, image.cols, CV_32F);
    gradientMagnitude.create(image.rows, image.cols, CV_32F);
    laplacian.create(image.rows, image.cols, CV_32F);
    
    //allocate objects for converting gradient images for display
    displayDX.create(dX.rows, dX.cols, CV_8UC1);
    displayDY.create(dY.rows, dY.cols, CV_8UC1);
    displayMagnitude.create(gradientMagnitude.rows, gradientMagnitude.cols, CV_8UC1);
    displayLaplacian.create(laplacian.rows, laplacian.cols, CV_8UC1);
    
    
    // Create a new windows
    namedWindow( "Image View", 1 );
    
    //attach a mouse click callback to the window
    setMouseCallback("Image View", onClickCallback, NULL);
    //create a slider in the window
    createTrackbar( "Smoothing", "Image View", &smoothSlider, smoothSliderMax, onTrackbar );
    
    //all of the stuff gets computed in the onTrackbar function so that things get recomputed
    //when you apply different levels of smoothing
    //Here, we call it manually for initialization
    onTrackbar(0, NULL);
    
    //two more windows for displaying the derivative images
    namedWindow("DX", 1);
    namedWindow("DY", 1);
    namedWindow("Gradient Magnitude", 1);
    namedWindow("Laplacian", 1);
    
    //display the images and wait for 33 milliseconds in a loop, to
    //allow us to refresh the displayed image when we click at locations in the image
    while(1)
    {
        imshow("Image View", displayImage);
        imshow("DX", displayDX);
        imshow("DY", displayDY);
        imshow("Gradient Magnitude", displayMagnitude);
        imshow("Laplacian", displayLaplacian);
        char key=waitKey(33);
        if(key == 'Q' || key == 'q')
        {
            break;
        }
        if(key == 'S' || key == 's')
        {
            char filename[512];
            snprintf(filename, "outputFile_%03d.png", outputCounter);
            imwrite(filename, displayImage);
            cout<<"Image Saved: "<<filename<<endl;
            outputCounter++;
        }
    }
    
    return 0;
}


void gradient_FiniteDifferences_v1(Mat& image, Mat& dX, Mat& dY)
{
    //in two double for loops, calculate the difference between consecutive elements
    if(image.empty())
    {
        cout<<"Error: image is unallocated"<<endl;
        return;
    }
    
    if(dX.empty())
    {
        dX=Mat::zeros(image.rows, image.cols, CV_32FC1);
    }
    
    if(dY.empty())
    {
        dY=Mat::zeros(image.rows, image.cols, CV_32FC1);
    }
    
    for(int row=1; row<image.rows-1; row++)
    {
        unsigned char* imgPtr = image.ptr<unsigned char>(row);
        unsigned char* imgPtr_above = image.ptr<unsigned char>(row-1);
        float* dxPtr = dX.ptr<float>(row);
        float* dyPtr = dY.ptr<float>(row);
        for(int col=1; col<image.cols-1; col++)
        {
            //This is terrible! It isn't even centered on the pixel that we are working on!
            dxPtr[col] = imgPtr[col+1]-imgPtr[col];
            dyPtr[col] = imgPtr_above[col] - imgPtr[col];
        }
    }
}

// here is one way of calculating a measure of the derivative with wider spatial support
void gradient_FiniteDifferences_v2(Mat& image, Mat& dX, Mat& dY)
{
    if(image.empty())
    {
        cout<<"Error: image is unallocated"<<endl;
        return;
    }
    
    if(dX.empty())
    {
        dX=Mat::zeros(image.rows, image.cols, CV_32FC1);
    }
    
    if(dY.empty())
    {
        dY=Mat::zeros(image.rows, image.cols, CV_32FC1);
    }
    
    for(int row=1; row<image.rows-1; row++)
    {
        unsigned char* imgPtr = image.ptr<unsigned char>(row);
        unsigned char* imgPtr_above = image.ptr<unsigned char>(row-1);
        unsigned char* imgPtr_below = image.ptr<unsigned char>(row+1);
        float* dxPtr = dX.ptr<float>(row);
        float* dyPtr = dY.ptr<float>(row);
        for(int col=1; col<image.cols-1; col++)
        {
            //At least this is centered, but good grief. So ugly!
            dxPtr[col] = imgPtr_above[col+1] + 2*imgPtr[col+1] + imgPtr_below[col+1]
            - imgPtr_above[col-1] - 2*imgPtr[col-1] - imgPtr_below[col-1];
            
            dyPtr[col] = imgPtr_above[col-1] + 2*imgPtr_above[col] + imgPtr_above[col+1]
            - imgPtr_below[col-1] - 2*imgPtr_below[col] - imgPtr_below[col+1];
        }
    }
}

void gradient_Convolution(Mat& image, Mat& dX, Mat& dY)
{
    Mat kernelY = Mat::zeros(3,3, CV_32F);
    kernelY.at<float>(0, 0) = 1;
    kernelY.at<float>(0, 1) = 2;
    kernelY.at<float>(0, 2) = 1;
    kernelY.at<float>(2, 0) = -1;
    kernelY.at<float>(2, 1) = -2;
    kernelY.at<float>(2, 2) = -1;
    
    Mat kernelX;
    kernelY.copyTo(kernelX);
    transpose(kernelX, kernelX);
    
    convolutionByHand(image, dX, kernelX);
    convolutionByHand(image, dY, kernelX);
    
}

//Instead of writing a new function for every set of coefficients we want to use, we
//can put the coefficients in a separate matrix and then do a quadruple for loop
//This process is called "convolution", and the set of coefficients is called a
//"kernel", "filter", or "mask"
//This is the logic of convolution
//Note: I didn't actually test that the function works. I just wanted to illustrate
void convolutionByHand(Mat& image, Mat& result, Mat& coefficients)
{
    //assume the size of the coefficients is odd
    int coeffRows = (coefficients.rows-1)/2;
    int coeffCols = (coefficients.cols-1)/2;
    
    //change the iteration conditions so that we don't try to access memory incorrectly
    for(int row=coeffRows; row<image.rows-coeffRows-1; row++)
    {
        for(int col=coeffCols; col<image.cols-coeffCols-1; col++)
        {
            //initialize the result
            float* resultPtr = result.ptr<float>(row);
            resultPtr[col] = 0;
            
            //go through all the elements of the coefficients, and accumulate the result
            for(int dr=-coeffRows; dr<coeffRows; dr++)
            {
                for(int dc=coeffCols; dc<coeffCols; dc++)
                {
                    unsigned char* imgPtr = image.ptr<unsigned char>(row+dr);
                    
                    //dr+coeffRows so that we don't try to use negative indexes
                    float* coefPtr = coefficients.ptr<float>(dr+coeffRows);
                    resultPtr[col] += imgPtr[col+dc]*coefPtr[dc+coeffCols];
                }
            }
        }
    }
}


void gradient_OpenCV_v1(Mat& image, Mat& dX, Mat& dY)
{
    //oh, phew. I knew there had to be an easier way
    //
    //http://docs.opencv.org/modules/imgproc/doc/filtering.html?highlight=sobel#sobel
    //calculate the gradients with OpenCV Sobel() function
    Sobel( image, dX, CV_32F, 1, 0, 3, 1, 0, BORDER_DEFAULT );
    Sobel( image, dY, CV_32F, 0, 1, 3, 1, 0, BORDER_DEFAULT );
}

void gradient_OpenCV_v2(Mat& image, Mat& dX, Mat& dY)
{
    // The most general way of computing any linear combinations of elements of your image
    // is with a convolution or filtering operation.
    //
    // The set of coefficients are stored in another matrix, which is referred to as a
    // "kernel", "filter", "operator" or "mask" (a convolution mask, not to be confused with a binary mask)
    //
    //calculate the gradients with more general Convolution function
    //http://docs.opencv.org/modules/imgproc/doc/filtering.html?highlight=filter2d#filter2d
    
    //construct the sobel kernels. This is just an example of how to construct a kernel. For using the
    //Sobel operators to compute derivatives, you should really use the Sobel() function
    Mat kernelY = Mat::zeros(3,3, CV_32F);
    kernelY.at<float>(0, 0) = 1;
    kernelY.at<float>(0, 1) = 2;
    kernelY.at<float>(0, 2) = 1;
    kernelY.at<float>(2, 0) = -1;
    kernelY.at<float>(2, 1) = -2;
    kernelY.at<float>(2, 2) = -1;
    
    Mat kernelX;
    kernelY.copyTo(kernelX);
    transpose(kernelX, kernelX);
    
    filter2D(image, dX, CV_32F, kernelX);
    filter2D(image, dY, CV_32F, kernelX);
    
}

void gradientSobel(Mat& image, Mat& dX, Mat& dY, Mat& magnitude)
{
    //http://docs.opencv.org/modules/imgproc/doc/filtering.html?highlight=sobel#sobel
    //calculate the gradients with OpenCV Sobel() function
    Sobel( image, dX, CV_32F, 1, 0, 3, 1.0/8.0, 0, BORDER_DEFAULT );
    Sobel( image, dY, CV_32F, 0, 1, 3, 1.0/8.0, 0, BORDER_DEFAULT );
    
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
    
    //cout<<"Min and Max values for conversion: "<<minVal<<' '<<maxVal<<endl;
    
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
    //int numchannels = mask.channels();
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
    
    //make a square, centered on the clicked location
    for(int row=y-10; row<y+10; row++)
    {
        if(row < 0 || row >= maskImage.rows)
        {
            continue;
        }
        for(int col=x-10; col<x+10; col++)
        {
            if(col < 0 || col >= maskImage.cols)
            {
                continue;
            }
            maskImage.at<unsigned char>(row,col)=255;
        }
    }
    
    markImageForDisplay(image, displayImage, maskImage);
    double dx = dX.at<float>(y,x);
    double dy = dY.at<float>(y,x);
    //double magnitude = sqrt(dx*dx + dy*dy);
    //double direction = atan2(dy, dx)*180.0/M_PI;
    cout<<"Point: ("<<x<<", "<<y<<"). Gradient = ("<<dx<<", "<<dy<<")"<<endl;
}

void onTrackbar(int value, void* data)
{
    original.copyTo(image);
    smoothImage(image, value);
    
    //compute the gradient four ways
    //gradient_FiniteDifferences_v1(image, dX, dY);
    //gradient_FiniteDifferences_v2(image, dX, dY);
    //gradient_OpenCV_v1(image, dX, dY);
    //gradient_OpenCV_v2(image, dX, dY);
    
    gradientSobel(image, dX, dY, gradientMagnitude);
    Laplacian(image, laplacian, CV_32F);
    laplacian = abs(laplacian);
    markImageForDisplay(image, displayImage, maskImage);
    convertGradientImageForDisplay(dX, displayDX);
    convertGradientImageForDisplay(dY, displayDY);
    convertGradientImageForDisplay(gradientMagnitude, displayMagnitude);
    convertGradientImageForDisplay(laplacian, displayLaplacian);
}
