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
    Mat binary=imread(cn);
    Mat mark(binary.cols,binary.rows,CV_8UC1);
    long int nPix=binary.rows*binary.cols;
    Stack<Scalar> s((int)nPix);
    Scalar test=s.pop();
    uchar *rowPtr;
    cout<<"Begin pushing and popping all pixels in binary image onto stack"<<endl;
    for(int y=0;y<binary.rows;y++)
    {
        rowPtr=binary.ptr<uchar>(y);
        for(int x=0;x<binary.cols;x++)
        {
            s.push(Scalar(*rowPtr,*(rowPtr + 1),*(rowPtr + 2),0.0f));
        }
    }
    Scalar pix;
    for(int i=0;i<nPix;i++)
    {
        pix=s.pop();
        cout<<"Pixel("<<pix[0]<<","<<pix[1]<<","<<pix[2]<<","<<pix[3]<<")"<<endl;
    }
    //std::cout<<"End of pushing and popping all pixels in binary image onto stack"<<endl;
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

