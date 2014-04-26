//
//  FloodFillMethods.cpp
//  FloodFill
//
//  Created by Don Johnson on 4/24/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#include "FloodFillMethods.h"

using namespace std;
using namespace cv;

void testStack(Mat& binary, string dataDir)
{
    imshow("binary",binary);
    imwrite(dataDir+"outbinary.png",binary);
    long int nPix=binary.rows*binary.cols;
    Stack<PIXEL> s((int)nPix);
    PIXEL pixel;
    int channels=binary.channels();
    cout<<"Begin pushing and popping all pixels in binary image onto stack"<<endl;
    for(int y=0;y<binary.rows;y++)
    {
        for(int x=0;x<binary.cols;x++)
        {
            pixel.pt=Point(x,y);
            pixelInBounds(binary, pixel.pt);
            getPixel_8UC1(binary, pixel);
            s.push(pixel);
            printf("Pushed pixel(%d,%d):(%0.0f,%0.0f,%0.0f,%0.0f)\n",pixel.pt.x,pixel.pt.y,pixel.val[0],pixel.val[1],pixel.val[2],pixel.val[3]);
        }
    }
    Mat test(binary.rows,binary.cols,CV_8UC1);
    channels=test.channels();
    for(long i=0;i<nPix;i++)
    {
        pixel=s.pop();
        printf("Popped pixel(%d,%d)(%0.0f,%0.0f,%0.0f,%0.0f)\n",pixel.pt.x,pixel.pt.y,pixel.val[0],pixel.val[1],pixel.val[2],pixel.val[3]);
        pixelInBounds(test, pixel.pt);
        setPixel_8UC1(pixel,test);
    }
    std::cout<<"End of pushing and popping all pixels in binary image onto stack"<<endl;
    imshow("test",test);
    imwrite(dataDir+"outtest.png",test);
    waitKey();
}