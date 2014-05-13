// CS585 Lab 1, Part 2
// Efficiency Considerations and Overflow
//
// This program reads an image from a file, and then performs a loop where 
// it adds one to every element of the image and displays the image.
//
// Adding one to every element of the image is done in four different ways. 
// The code is instrumented with timers to demonstrate the dramatic difference 
// in efficiency between these logically equivalent functions.
//
// We also explore the idea of overflow and what happens when you attempt to set 
// elements of the image to values outside of the range of [0 255]
//
// Copyright 2014 Diane H. Theriault
//
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

//#include "CStopWatch.h"

using namespace std;
using namespace cv;


void addOne_LinearIndex_v1(Mat& image);
void addOne_LinearIndex_v2(Mat& image);
void addOne_LinearIndex_v3(Mat& image);
void addOne_RowPtr(Mat& image);

int main(int argc, char* argv[])
{
    string dataDir="/Users/donj/workspace/cs585/Lab1/Data/";
    
    if (argc < 2)
    {
        cout<<"Usage: Lab1 imageName"<<endl;
        return 0;
    }

    Mat image;
    image = imread(dataDir+argv[1]);

    namedWindow( "Image View", 1 );
    imshow("Image View", image);


    //CStopWatch timer;
    image = imread(dataDir+argv[1]);
    //timer.startTimer();
    clock_t startTime = clock();
    
    for(int i=0; i<255; i++)
    {
        addOne_LinearIndex_v1(image);
        imshow("Image View", image);

        if(waitKey(1) == 'q')
        {
            break;
        }        
    }
    //timer.stopTimer();
    //double linearIndex_v1_Time = timer.getElapsedTime();
    cout << "addOne_LinearIndex_v1 " << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;

    image = imread(dataDir+argv[1]);
    //timer.startTimer();
    startTime = clock();

    for(int i=0; i<255; i++)
    {
        addOne_LinearIndex_v2(image);
        imshow("Image View", image);

        if(waitKey(1) == 'q')
        {
            break;
        }        
    }
    //timer.stopTimer();
    //double linearIndex_v2_Time = timer.getElapsedTime();
    cout << "addOne_LinearIndex_v2 " << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;

    startTime = clock();
    image = imread(dataDir+argv[1]);
    //timer.startTimer();
    for(int i=0; i<255; i++)
    {
        addOne_LinearIndex_v3(image);
        imshow("Image View", image);

        if(waitKey(1) == 'q')
        {
            break;
        }        
    }
    //timer.stopTimer();
    //double linearIndex_v3_Time = timer.getElapsedTime();
    cout << "addOne_LinearIndex_v3 " << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;

    image = imread(dataDir+argv[1]);
    //timer.startTimer();
    startTime = clock();

    for(int i=0; i<255; i++)
    {
        addOne_RowPtr(image);
        imshow("Image View", image);

        if(waitKey(1) == 'q')
        {
            break;
        }        
    }
    //timer.stopTimer();
    //    double rowPtr_Time = timer.getElapsedTime();
    cout << "addOne_RowPtr " << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;

    return 0;
}

//This function adds one to every element of the image, using a style similar to Part 1
void addOne_LinearIndex_v1(Mat& image)
{
    if(image.empty())
    {
        cout<<"Error: image is unallocated"<<endl;
        return;
    }

    for(int row=0; row<image.rows; row++)
    {
        //cout << "Row: "<<row<<endl;
        for(int col=0; col<image.cols; col++)
        {
            for(int channel=0; channel<image.channels(); channel++)
            {
                //note that the number of channels in the image is queried repeatedly 
                //using the channels() method
                int index = row*(image.step[0]) + col*image.channels() + channel;
                image.data[index] ++;
            }
        }
    }
}

//This function also adds one to every element in the array, but the number of channels 
//in the image is stored in a variable at the beginning of the function, rather than being 
//queried repeatedly in the innermost loop
void addOne_LinearIndex_v2(Mat& image)
{
    if(image.empty())
    {
        cout<<"Error: image is unallocated"<<endl;
        return;
    }

    //note that the number of channels in the image is stored in a variable, 
    //rather than being queried repeatedly inside the inner loop.
    int channels = image.channels();
    for(int row=0; row<image.rows; row++)
    {
        for(int col=0; col<image.cols; col++)
        {
            for(int channel=0; channel<channels; channel++)
            {
                int index = row*(image.step[0]) + col*channels + channel;
                image.data[index] ++;
            }
        }
    }
}

//This function also adds one to every element of the image, but in this version, the 
//index of the start of the row is calculated once for the start of each row, rather 
//than being recalculated inside the innermost loop
//
// Further gains can be had if you only use additions to run along all elements of the 
// image, rather than calculating the indexes using excess multiplications.
void addOne_LinearIndex_v3(Mat& image)
{
    if(image.empty())
    {
        cout<<"Error: image is unallocated"<<endl;
        return;
    }

    int channels = image.channels();
    for(int row=0; row<image.rows; row++)
    {
        //the index for the start of the row is only calculated once per row
        int rowIndex = row*image.step[0];
        for(int col=0; col<image.cols; col++)
        {
            for(int channel=0; channel<channels; channel++)
            {
                int index = rowIndex + col*channels + channel;
                image.data[index] ++;
            }
        }
    }
}

//This function also adds one to every element of the image, but it does it using row pointers.
void addOne_RowPtr(Mat& image)
{
    if(image.empty())
    {
        cout<<"Error: image is unallocated"<<endl;
        return;
    }

    int channels = image.channels();
    for(int row=0; row<image.rows; row++)
    {
        unsigned char* rowPtr = image.ptr<unsigned char>(row);
        for(int col=0; col<image.cols; col++)
        {
            for(int channel=0; channel<channels; channel++)
            {
                int index = col*channels + channel;
                rowPtr[index] ++;
            }
        }
    }
}

