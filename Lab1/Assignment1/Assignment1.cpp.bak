// CS585 Assignment 1
// Color-based thresholding
//
// In this lab, you will practice manipulating images by computing 
// a mask by thresholding the red channel of an input image

#include "stdafx.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

void thresholdChannel(Mat& image, Mat& mask, int channelIndex, int threshold)
{
    // Coding defensively and including error checking like this in all of your functions
    // can be a big help for preventing memory-bsaed bugs that lead to useless error messages
    // and can be hard to track down
    if(image.empty() || mask.empty())
    {
        cout<<"Error: image is unallocated"<<endl;
        return;
    }
    if(image.rows != mask.rows || image.cols != mask.cols)
    {
        cout<<"Images do not have the same dimensions"<<endl;
        return;
    }

    int channels = image.channels();
    if(channelIndex >= channels)
    {
        cout<<"Invalid channel index"<<endl;
        return;
    }

    if(threshold > 255)
    {
        cout<<"Invalid threshold value for unsigned char"<<endl;
        return;
    }

    //Here, add your code to create a mask image by thresholding the red values of all
    //of the pixels in the image.
    //For any pixel in the source image where the red value exceeds the threshold, 
    //the corresponding pixel in the mask image should be set to 255. Otherwise, it 
    //should be set to zero.
	    //If the images have the same number of rows and columns (verified above), then you 
    //can access both images in the same loop
    for(int row=0; row<image.rows; row++)
    {
        unsigned char* imagePtr = image.ptr<unsigned char>(row);
        unsigned char* channelPtr = mask.ptr<unsigned char>(row);

        for(int col=0; col<image.cols; col++)
        {
            // the index into the image Mat object is different than the index into the 
            // channel Mat object because the channel Mat was created with one channel -- 
            // grayscale (CV_8UC1) instead of BGR color
            int index = col*channels + channelIndex;
            channelPtr[col] = imagePtr[index] >= threshold ? 255 : 0;
        }
    }
}

// This function is used to find the red egg and reject the yellow egg
void thresholdRedGreenRatio(Mat& image, Mat& mask)
{
    // Coding defensively and including error checking like this in all of your functions
    // can be a big help for preventing memory-bsaed bugs that lead to useless error messages
    // and can be hard to track down
    if(image.empty() || mask.empty())
    {
        cout<<"Error: image is unallocated"<<endl;
        return;
    }
    if(image.rows != mask.rows || image.cols != mask.cols)
    {
        cout<<"Images do not have the same dimensions"<<endl;
        return;
    }

    int channels = image.channels();
	float twoThirds = 2.0/3;
	float oneThird = 1.0/3;
    for(int row=0; row<image.rows; row++)
    {
        unsigned char* imagePtr = image.ptr<unsigned char>(row);
        unsigned char* channelPtr = mask.ptr<unsigned char>(row);

        for(int col=0; col<image.cols; col++)
        {
            // the index into the image Mat object is different than the index into the 
            // channel Mat object because the channel Mat was created with one channel -- 
            // grayscale (CV_8UC1) instead of BGR color
            int indexGreen = col*channels + 1; // red
            int indexRed = col*channels + 2; // green
			int redPlusGreen = imagePtr[indexRed] + imagePtr[indexGreen]; 
			float ratio = imagePtr[indexRed]/(redPlusGreen + 0.1);
            channelPtr[col] = ratio > oneThird && redPlusGreen > 300 ? 128 : 0; 
            channelPtr[col] = ratio > twoThirds && redPlusGreen > 100  ? 255 : channelPtr[col];
            int indexBlue = col*channels; // blue
            channelPtr[col] = imagePtr[indexBlue] > 100  ? 0 : channelPtr[col];
        }
    }
}


int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cout<<"Usage: Assignment1 imageName redThreshold"<<endl;
    }

    // Read in the image whose filename is given on the command line
    Mat image;
    image = imread(argv[1]); 

    // Construct an extra image containing only one channel
    Mat redThreshold(image.rows, image.cols, CV_8UC1);

    // Display the original image
    namedWindow( "Image View", 1 );
    imshow("Image View", image);
    waitKey(1);

    // Read in the threshold to use from the command line
    // atoi is a C command for converting from a string to an integer
    int threshold = atoi(argv[2]);
    namedWindow( "Threshold View", 1 );
    
    //
    //
    // You must define this function to perform the thresholding operation and define the mask
    thresholdChannel(image, redThreshold, 2, threshold);
    //thresholdRedGreenRatio(image, redThreshold);
    //
    //
    //

    //display the image and save
    imwrite("redEgg.thresholded.175.png", redThreshold);
    //imwrite("redThresholdAppliedToYellowEgg.png", redThreshold);
    //imwrite("detectRedAndYellowEgg.png", redThreshold);
    //imwrite("detectRedAndYellowOnPenguin.png", redThreshold);
    imshow("Threshold View", redThreshold);

    waitKey(0);

    return 0;
}

