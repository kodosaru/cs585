// Lab3_Part2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <string>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

Mat originalImage;
vector<Mat> scaleSpace;
vector<Mat> DoG;
Mat displayScale;;
Mat displayDoG;

bool convertGradientImageForDisplay(Mat& input, Mat& output);
void buildScaleSpace(Mat& input, vector<Mat>& output, int numLayers, double sigma);
void buildDoG(vector<Mat>& input, vector<Mat>& output);

//a callback function to pass to the slider bar to threshold the red object
//this is the function that actually makes the call to find the red object
void onTrackbar(int value, void* data);

int main(int argc, char* argv[])
{

    if(argc <= 1)
    {
        cout<<"Please provide a filename of an image"<<endl;
        return 0;
    }

    //read in the image
    originalImage = imread(argv[1], 0);
    displayScale.create(originalImage.rows, originalImage.cols, CV_8UC1);
    displayDoG.create(originalImage.rows, originalImage.cols, CV_8UC1);
    double numScalesPerOctave=3;
    double numOctaves = 4;
    double k = pow(2, 1.0/numScalesPerOctave);
    int numLayers = numOctaves * numScalesPerOctave+2;
    int layer=0;
    buildScaleSpace(originalImage, scaleSpace, numLayers, k);
    buildDoG(scaleSpace, DoG);

    namedWindow("Image Window", 1);
    createTrackbar("Layer", "Image Window", &layer, numLayers-2, onTrackbar, NULL);
    namedWindow("Scale Window", 1);
    namedWindow("DoG Window", 1);
    onTrackbar(0, NULL); //to initialize the display images

    while(1 == 1)
    {
        imshow("Image Window", originalImage);
        imshow("Scale Window", displayScale);
        imshow("DoG Window", displayDoG);
        char key = waitKey(33);
        if(key == 'q')
        {
            break;
        }
        if(key == ' ')
        {
//            imwrite("Lab1_Part2_result.png", displayImage);
        }
    }
}

void buildDoG(vector<Mat>& input, vector<Mat>& output)
{
    int numLayers = input.size();
    output.clear();
    for(int i=1; i<numLayers; i++)
    {
        output.push_back(Mat(input[0].rows, input[0].cols, CV_32FC1));
        subtract(input[i], input[i-1], output[i-1]);
    }
}


void buildScaleSpace(Mat& input, vector<Mat>& output, int numLayers, double k)
{
    output.clear();
    output.push_back(Mat(input.rows, input.cols, CV_32FC1));
    input.convertTo(output[0], CV_32FC1);
    double currentSigma=1;
    for(int i=1; i<numLayers; i++)
    {
        double newSigma = currentSigma * k;
        double dsigma = sqrt(newSigma*newSigma - currentSigma*currentSigma);
        currentSigma = newSigma;
        output.push_back(Mat(input.rows, input.cols, CV_32FC1));
        GaussianBlur(output[i-1], output[i], cv::Size(ceil(dsigma)*4+1, ceil(dsigma)*4+1), dsigma, dsigma, BORDER_REPLICATE);
    }
}

void onTrackbar(int layer, void* data)
{
    convertGradientImageForDisplay(scaleSpace[layer], displayScale);
    convertGradientImageForDisplay(DoG[layer], displayDoG);
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
    temp = ((temp-minVal) / (maxVal-minVal))*255;
    temp.convertTo(output, CV_8UC1);
    cout<<minVal<<' '<<maxVal<<endl;
    return true;
}
