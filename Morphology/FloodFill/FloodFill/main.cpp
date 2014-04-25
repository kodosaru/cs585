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
    Mat mark(binary.cols,binary.rows,CV_8UC1);
    
    // Confirm stack routines are working properly before running flood fill
    //testStack(binary,dataDir);
    
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
            pixPtr=rowPtr+(x*channels);
            pix.val=Scalar(*pixPtr,*(pixPtr + 1),*(pixPtr + 2),0.0f);
            s.push(pix);
            if(DEBUG)
                printf("Pushed pixel(%d,%d): %0.0f,%0.0f,%0.0f,%0.0f\n",pix.pt.x,pix.pt.y,pix.val[0],pix.val[1],pix.val[2],pix.val[3]);
        }
    }

    
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
