// CS585 Lab 1, Part 1
// Image Representation in OpenCV
//
// This program reads an image from a file, then displays the image.
// The image is then displayed as tinted with Blue, Green, and Red.
// We will demonstrate two ways performing this operation.
// Then, the program displays the Blue, Green, and Red channels of the image
//
// Copyright 2014 Diane H. Theriault
//
//
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

// These are the function signatures (declarations) for some functions we will use 
// in this part of the lab. The function bodies (definitions) are underneath the 
// main() function.
//
// In C++, functions must be declared or defined in the file before you attempt you use them.
// To help keep your file organized, you can provide a declaration (just the function signature) 
// before you provide the body of the function. The arguments and return value of the declaration 
// and definition must match exactly.
void tintImage_LinearIndex(Mat& image, int channelIndex);
void tintImage_RowPtr(Mat& image, int channelIndex);
void getChannel(Mat& image, Mat& channel, int channelIndex);

int main(int argc, char* argv[])
{
    string dataDir="/Users/donj/workspace/cs585/Lab1/Data/";
    //Tip : Avoid hard-coding pathnames by using command-line arguments instead
    //First check that a command-line argument has been given
    if (argc < 2)
    {
        cout<<"Usage: Lab1 imageName"<<endl;
        return 0;
    }

    // The array of command line arguments is stored in the variable argv
    // Each command line argument is a C-style string (char*)
    // The first argument, argv[0] is always the name of the program, and is
    // populated by the operating system
    // Arguments provided by the user start in argv[1]
    Mat image;
    image = imread(dataDir+argv[1]);

    //the empty() method of the Mat class determines if the class has been successfully 
    //initialized
    if(image.empty())
    {
        cout<<"Unable to open the image file: "<<argv[1]<<endl;
        return 0;
    }


    // Create a new window, display the image, and wait for the user to press a key
    namedWindow( "Image View", 1 );
    imshow("Image View", image);
    waitKey(0);

    // Tint the image blue, display the image, and write out the resulting image
    tintImage_LinearIndex(image, 0);
    imshow("Image View", image);
    imwrite("tintedBlue.jpg", image);
    waitKey(0);

    // Re-initialize the image data, tint the image green, display the image, and 
    // write out the resulting image
    image = imread(dataDir+argv[1]);
    tintImage_RowPtr(image, 1);
    imshow("Image View", image);
    imwrite("tintedGreen.jpg", image);
    waitKey(0);

    // Re-initialize the image data, tint the image red, display the image, and 
    // write out the resulting image
    image = imread(dataDir+argv[1]);
    tintImage_RowPtr(image, 2);
    imshow("Image View", image);
    imwrite("tintedRed.jpg", image);
    waitKey(0);

    // Re-initialize the image data, extract the blue channel, display the image, 
    // and write out the resulting image    
    namedWindow( "Channel View", 1 );
    image = imread(dataDir+argv[1]); 

    //Mat objects can be constructed by specifying a size (rows and columns), as well as a type.
    // CV_8UC1 means 8 bit, unsigned, 1 channel
    Mat blueChannel(image.rows, image.cols, CV_8UC1);
    getChannel(image, blueChannel, 0);
    imshow("Image View", image);
    imshow("Channel View", blueChannel);
    imwrite("blueChannel.jpg", blueChannel);
    waitKey(0);

    // Re-initialize the image data, extract the green channel, display the image, 
    // and write out the resulting image    
    image = imread(dataDir+argv[1]);
    Mat greenChannel(image.rows, image.cols, CV_8UC1);
    getChannel(image, greenChannel, 1);
    imshow("Image View", image);
    imshow("Channel View", greenChannel);
    imwrite("greenChannel.jpg", greenChannel);
    waitKey(0);

    // Re-initialize the image data, extract the red channel, display the image, and write out the resulting image    
    image = imread(dataDir+argv[1]);
    Mat redChannel(image.rows, image.cols, CV_8UC1);
    getChannel(image, redChannel, 2);
    imshow("Image View", image);
    imshow("Channel View", redChannel);
    imwrite("redChannel.jpg", redChannel);
    waitKey(0);

    return 0;
}

// Color images have 3 channels. By convention, the first channel is blue, the 
// second is green, and the third is red. Usually, image data is represented as 
// unsigned char's (8 bits), which take on values between 0 and 255. 
//
// This function tints the image by setting every element of one of the channels to 255.
//
// In C++, references (denoted with an &) are like pointers, but can be accessed like 
// regular objects, using the . notation. The object owned by the caller will be 
// modified if the object is modified inside the function.
//
// The data stored in Mat objects can be accessed as a flat array by 
// calculating the index of the array element you want to access.
//
void tintImage_LinearIndex(Mat& image, int channelIndex)
{
    if(image.empty())
    {
        cout<<"Error: image is unallocated"<<endl;
        return;
    }

    //image.rows is the number of rows in the image (the height)
    for(int row=0; row<image.rows; row++)
    {
        //image.cols is the number of columns in the image (the width)
        for(int col=0; col<image.cols; col++)
        {
            //the number of channels (BGR vs gray) is accessed by calling the channels() method
            //image.step is the number of bytes between successive rows in the image, usually 
            //equal to image.cols * image.channels(), but not always.
            int index = row*(image.step[0]) + col*image.channels() + channelIndex;

            //the naked pointer to the data of the Mat object is accessed through the "data" 
            //member variable
            image.data[index] = 255;
        }
    }
}


// The data stored in Mat objects can also be accessed row by row. This is convenient
// when the data in the Mat data structure is really some type other than an unsigned char
void tintImage_RowPtr(Mat& image, int channelIndex)
{
    if(image.empty())
    {
        cout<<"Error: image is unallocated"<<endl;
        return;
    }

    int channels = image.channels();
    for(int row=0; row<image.rows; row++)
    {
        //the .ptr method of the Mat class returns a pointer to the specifed row.
        //the type of the returned pointer is given inside the angle brackets
        //
        // The reason you might prefer to use this notation is because
        // Mat objects can be used to store other types of data too, not just unsigned char's.
        // However, the data member variable is always of unsigned char.
        unsigned char* rowPtr = image.ptr<unsigned char>(row);
        for(int col=0; col<image.cols; col++)
        {
            // Once you have a pointer to the beginning of the row, you still need to calculate
            // the index of the element of the row that you want to access
            int index = col*channels + channelIndex;
            rowPtr[index] = 255;
        }
    }
}

void getChannel(Mat& image, Mat& channel, int channelIndex)
{
    if(image.empty() || channel.empty())
    {
        cout<<"Error: image is unallocated"<<endl;
        return;
    }
    if(image.rows != channel.rows || image.cols != channel.cols)
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


    //If the images have the same number of rows and columns (verified above), then you 
    //can access both images in the same loop
    for(int row=0; row<image.rows; row++)
    {
        unsigned char* imagePtr = image.ptr<unsigned char>(row);
        unsigned char* channelPtr = channel.ptr<unsigned char>(row);

        for(int col=0; col<image.cols; col++)
        {
            // the index into the image Mat object is different than the index into the 
            // channel Mat object because the channel Mat was created with one channel -- 
            // grayscale (CV_8UC1) instead of BGR color
            int index = col*channels + channelIndex;
            channelPtr[col] = imagePtr[index];
        }
    }
}
