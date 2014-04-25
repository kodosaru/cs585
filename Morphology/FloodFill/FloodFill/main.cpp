//
//  main.cpp
//  FloodFill
//
//  Created by Don Johnson on 4/24/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//
#define DEBUG 1

#include <iostream>
#include "FloodFillMethods.h"
#include "Stack.hpp"

using namespace std;
using namespace cv;

// Begin flood fill algorithm (depthFirstTransversal)
void floodFill(Mat& image, Mat& regions)
{
    regions=Scalar(0);
    Stack<PIXEL> s((int)image.rows*image.cols);
    uchar region=1; // initialize region counter
    cout<<"Starting region "<<(short)region<<" transversal"<<endl;
    uchar *rowPtr;
    uchar *pixPtr;
    PIXEL imagePixel, regionsPixel;
    int channels=image.channels();

    for(int y=0;y<image.rows;y++)
    {
        // Retrieve pixel from image
        rowPtr=image.ptr<uchar>(y);
        for(int x=0;x<image.cols;x++)
        {
            imagePixel.pt=Point(x,y);
            pixPtr=rowPtr+(x*channels);
            imagePixel.val=Scalar(*pixPtr,*(pixPtr + 1),*(pixPtr + 2),0.0f);
            
            // Run this algorithm against every pixel in an image to mark all regions
            //• Depth-first Traversal(self)!
            //      Stack.push(self)!
            //–     While(!stack.empty())!
            //•         node = stack.pop()!
            //•         For each neighbor!
            //–             If not marked!
            //»                 Mark neighbor!
            //»                 Stack.push(neighbor)!

            // Push the pixel on to stack to start process of neighbor check
            if(regions.at<uchar>(imagePixel.pt.x,imagePixel.pt.y) > 0)
            {
                cout<<"Pixel("<<imagePixel.pt.x<<","<<imagePixel.pt.y<<") already assigned to region "<<(short)regions.at<uchar>(imagePixel.pt.x,imagePixel.pt.y)<<endl;
                break;
            }
            else
            {
                s.push(imagePixel);
            }
            
            // Define the regions map pixel value and set
            regionsPixel.val[0]=region;
            regionsPixel.pt=imagePixel.pt;
            setPixel1C(regions,regionsPixel);
            
            // Begin recursion through region of image pixel
            while(!s.isEmpty())
            {
                imagePixel=s.pop();
                // Check 8-way neighbors
                for(int i=-1;i<=+1;i+=2)
                {
                    for(int j=-1;j<=+1;j+=2)
                    {
                        if(!neighborsInBound(image,imagePixel.pt,8))
                        {
                            break;
                        }
                        else
                        {
                            s.push(imagePixel);
                            setPixel1C(regions,regionsPixel);
                        }
                    }
                } // End pixel neighborhood transversal
            } // End of region transversal
            
            // Increment region counter
            region++;
            cout<<"Starting region "<<(short)region<<" transversal"<<endl;
        } // End of image row transversal
    } // End of image column transversal
} // End of flood fill function

int main(int argc, const char * argv[])
{
    string dataDir="/Users/donj/workspace/cs585/Morphology/Data/Output/";
    int clusterCount=20;
    char cn[256];
    sprintf(cn,"%s%s%s%d%s",dataDir.c_str(),argv[1],"Binary",clusterCount,".png");
    Mat binary=imread(cn,CV_LOAD_IMAGE_GRAYSCALE);
    imshow("Binary",binary);
    
    // Create image of flood-fill regions
    Mat regions(binary.cols,binary.rows,CV_8UC1);
    if(DEBUG && regions.channels() != regions.channels())
        cout<< "Mark and binary matrices are different sizes"<<endl;
    
    // Confirm stack routines are working properly before running flood fill
    //testStack(binary,dataDir);
    //return 0;
    
    floodFill(binary, regions);
    imshow("Regions",regions);
    
    return 0;
}
