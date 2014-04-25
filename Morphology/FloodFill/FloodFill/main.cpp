//
//  main.cpp
//  FloodFill
//
//  Created by Don Johnson on 4/24/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#include <iostream>
#include "FloodFillMethods.h"
#include "Stack.hpp"


using namespace std;
using namespace cv;

int main(int argc, const char * argv[])
{
    string dataDir="/Users/donj/workspace/cs585/Morphology/Data/Output/";
    int clusterCount=20;
    char cn[256];
    sprintf(cn,"%s%s%s%d%s",dataDir.c_str(),argv[1],"Binary",clusterCount,".png");
    Mat binary=imread(cn,CV_LOAD_IMAGE_GRAYSCALE);
    Size size1=binary.size();
    long size2=binary.rows * binary.cols * binary.channels() * binary.elemSize();
    //Mat binary=imread(dataDir+"foregroundSmall.png");
    imshow("foreground",binary);
    //imwrite(dataDir+"outbinary.png",binary);
    
    Mat mark(binary.cols,binary.rows,CV_8UC1);
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
        //setPixel3C(pix, test);
        setPixel1C(pix, test);
        if(DEBUG)
            printf("Popped pixel(%d,%d): %0.0f,%0.0f,%0.0f,%0.0f\n",pix.pt.x,pix.pt.y,pix.val[0],pix.val[1],pix.val[2],pix.val[3]);
    }
    std::cout<<"End of pushing and popping all pixels in binary image onto stack"<<endl;
    imshow("test",test);
    imwrite(dataDir+"outtest.png",test);
    waitKey();
    //imshow("Binary",binary);
    //waitKey();

    //• Depth-first Traversal(self)!
    //– Stack.push(self)!
    //– While(!stack.empty())!
    //• node = stack.pop()!
    //• For each neighbor!
    //– If not marked!
    //» Mark neighbor!
    //» Stack.push(neighbor)!
    return 0;
}

inline void setPixel1C(PIXEL pixel, Mat img)
{
    uchar *rowPtr=img.ptr<uchar>(pixel.pt.y);
    rowPtr[pixel.pt.x]=(uchar)pixel.val[0];
    if(DEBUG && pixel.val[3] != 0.0f)
        cout<<"Scalar[3] not equal to zero"<<endl;
}

inline void setPixel3C(PIXEL pixel, Mat img)
{
    uchar *rowPtr=img.ptr<uchar>(pixel.pt.y);
    rowPtr[pixel.pt.x * 3]=(uchar)pixel.val[0];
    rowPtr[pixel.pt.x * 3 + 1]=(uchar)pixel.val[1];
    rowPtr[pixel.pt.x * 3 + 2]=(uchar)pixel.val[2];
    if(DEBUG && pixel.val[3] != 0.0f)
        cout<<"Scalar[3] not equal to zero"<<endl;
}