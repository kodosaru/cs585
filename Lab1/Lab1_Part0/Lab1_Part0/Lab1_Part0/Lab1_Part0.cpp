// CS585 Lab 1, Part 0
// Hello, CS585!
//
// This program reads an image from a file, then displays the image.
// The program waits for the user to press a key to end the program, 
// prints the key that was pressed, then exits.
//
// Copyright 2014 Diane H. Theriault
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

// the main() function is the entry point for all C++ programs
int main(int argc, char* argv[])
{
    string dataDir="/Users/donj/workspace/cs585/Lab1/Data/";
    //cout is the command to print things to the command line in C++
    //endl inserts an end of line character / line break
    cout<<"Hello CS585!"<<endl;

    //Mat is the OpenCV container for images and matrices
    Mat image;

    //imread is the OpenCV function for reading images from a file
    image = imread(dataDir+"HelloCS585.jpg");

    //namedWindow is the OpenCV function for creating a new window
    namedWindow( "Image View", 1 );

    //imshow is the OpenCV command for display an image in a window. 
    //The arguments are the name of the window, used with the namedWindow() function, 
    //and the image data, in a Mat structure
    imshow("Image View", image);
    char key = waitKey(0);

    //Using cout, different types of output can be chained together using the << operator
    cout<<"You pressed "<<key<<"."<<endl;

    //in C++, main returns an integer, which is usually 0 by convention
    return 0;
}

