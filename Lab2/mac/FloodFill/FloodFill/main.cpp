//
//  main.cpp
//  FloodFill
//
//  Created by Don Johnson on 1/28/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace std;
using namespace cv;

int main(int argc, const char * argv[])
{

    // insert code here...
    std::cout << "Hello, World!\n";
    
    
    //Create simple input image
    Point seed(4,4);
    Mat img = Mat::zeros(100,100,CV_8UC1);
    circle(img, seed, 20, Scalar(128),3);
    
    //Create a mask from edges in the original image
    Mat mask;
    Canny(img, mask, 100, 200);
    copyMakeBorder(mask, mask, 1, 1, 1, 1, BORDER_REPLICATE);
    
    //Fill mask with value 128
    uchar fillValue = 128;
    floodFill(img, mask, seed, Scalar(255) ,0, Scalar(), Scalar(), 4 | FLOODFILL_MASK_ONLY | (fillValue << 8));
    
    namedWindow( "Circle", 1);
    imshow("Circle",mask);
    
    return 0;
}

