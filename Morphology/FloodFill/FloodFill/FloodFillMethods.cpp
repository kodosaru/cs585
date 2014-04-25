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
    uchar *rowPtr;
    uchar *pixPtr;
    pixel pix;
    int channels=binary.channels();
    cout<<"Begin pushing and popping all pixels in binary image onto stack"<<endl;
    for(int y=0;y<binary.rows;y++)
    {
        rowPtr=binary.ptr<uchar>(y);
        for(int x=0;x<binary.cols;x++)
        {
            pix.pt=Point(x,y);
            pixelInBounds(binary, pix.pt);
            pixPtr=rowPtr+(x*channels);
            pix.val=Scalar(*pixPtr,*(pixPtr + 1),*(pixPtr + 2),0.0f);
            s.push(pix);
            if(DEBUG)
                printf("Pushed pixel(%d,%d): %0.0f,%0.0f,%0.0f,%0.0f\n",pix.pt.x,pix.pt.y,pix.val[0],pix.val[1],pix.val[2],pix.val[3]);
        }
    }
    //Mat test(binary.rows,binary.cols,CV_8UC3);
    Mat test(binary.rows,binary.cols,CV_8UC1);
    channels=test.channels();
    for(long i=0;i<nPix;i++)
    {
        PIXEL pix=s.pop();
        pixelInBounds(test, pix.pt);
        //setPixel3C(pix, test);
        setPixel1C(test, pix);
        if(DEBUG)
            printf("Popped pixel(%d,%d): %0.0f,%0.0f,%0.0f,%0.0f\n",pix.pt.x,pix.pt.y,pix.val[0],pix.val[1],pix.val[2],pix.val[3]);
    }
    std::cout<<"End of pushing and popping all pixels in binary image onto stack"<<endl;
    imshow("test",test);
    imwrite(dataDir+"outtest.png",test);
    waitKey();
}