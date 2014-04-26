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
    if(image.channels()>1 && image.elemSize()!=8)
    {
        cout<<"floodFill() only works on 8-bit grayscale input images"<<endl;
        return;
    }
    if(regions.channels()>1 && regions.elemSize()!=8)
    {
        cout<<"The region map fed to floodFill() must be an 16-bit grayscale image"<<endl;
        return;
    }
    Mat tempRegions(regions.size(),CV_8UC1);
    regions=Scalar(0);
    Stack<PIXEL> s((int)image.rows*image.cols);
    unsigned short region=1; // initialize region counter
    vector<int> regionCount;
    cout<<"Starting region "<<(unsigned short)region<<" transversal"<<endl;
    regionCount.push_back(0);
    PIXEL imagePixel, neighborPixel, regionsPixel;

    for(int y=0;y<image.rows;y++)
    {
        // Retrieve pixel from image
        for(int x=0;x<image.cols;x++)
        {
            imagePixel.pt=Point(x,y);
            getPixel_8UC1(image, imagePixel);
            
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
            regionsPixel.pt=imagePixel.pt;
            getPixel_16UC1(regions, regionsPixel);
            if(regionsPixel.val[0] > 0)
            {
                //cout<<"Pixel("<<imagePixel.pt.x<<","<<imagePixel.pt.y<<") already assigned to region "<<regionsPixel.val[0]<<endl;
                break;
            }
            else
            {
                s.push(imagePixel);

                // Define the regions map pixel value and set
                imagePixel.val[0]=region;
                setPixel_16UC1(imagePixel,regions);
                regionCount[region-1]++;
                printf("Mark pixel#%d(%d,%d)\n",regionCount[region-1],imagePixel.pt.x,imagePixel.pt.y);
            }
            
            // Begin recursion through region of image pixel
            while(!s.isEmpty())
            {
                imagePixel=s.pop();

                //printf("Checking neighborhood of pixel(%d,%d)=%0.0f\n",imagePixel.pt.x,imagePixel.pt.y,imagePixel.val[0]);
                // Check 8-way neighbors
                for(int deltaCol=-1;deltaCol<=+1;deltaCol++)
                {
                    for(int deltaRow=-1;deltaRow<=+1;deltaRow++)
                    {
                        // Set neighborPixel coordinates and value
                        neighborPixel.pt=Point(imagePixel.pt.x + deltaRow,imagePixel.pt.y + deltaCol);
                        //cout<<"Pixel("<<neighborPixel.pt.x<<","<<neighborPixel.pt.y<<")"<<endl;
                        if(!pixelInBounds(image,neighborPixel.pt))
                        {
                            // Neighbor pixel is out of image boundary
                            //cout<<"Pixel("<<neighborPixel.pt.x<<","<<neighborPixel.pt.y<<") out of bounds"<<endl;
                        }
                        else
                        {
                            regionsPixel.pt=neighborPixel.pt;
                            getPixel_16UC1(regions, regionsPixel);
                            if(regionsPixel.val[0] > 0)
                            {
                                //cout<<"Pixel("<<neighborPixel.pt.x<<","<<neighborPixel.pt.y<<") already assigned to region "<<regionsPixel.val[0]<<endl;
                            }
                            else
                            {
                                getPixel_8UC1(image, neighborPixel);
                                if(neighborPixel.val[0]==imagePixel.val[0])
                                {
                                    s.push(neighborPixel);

                                    // Define the regions map pixel value and set
                                    neighborPixel.val[0]=region;
                                    setPixel_16UC1(neighborPixel,regions);
                                    regionCount[region-1]++;
                                    printf("Mark pixel#%d(%d,%d)\n",regionCount[region-1],neighborPixel.pt.x,neighborPixel.pt.y);
                                }
                            }
                        }
                    } // End of row transversal
                } // End pixel column and neighborhood transversal
            } // End of region transversal
            
            // Increment region counter
            cout<<"Region "<<(unsigned short)region<<" count:"<<regionCount[region-1]<<endl;
            region++;
            regionCount.push_back(0);
            cout<<"Starting region "<<(unsigned short)region<<" transversal"<<endl;

            // Display region map
            regions.convertTo(tempRegions, CV_8UC1);
            threshold(tempRegions, tempRegions, 0, 255, THRESH_BINARY);
            imshow("Regions",tempRegions);

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
    Mat regions(binary.size(),CV_16UC1);
    if(DEBUG && regions.channels() != regions.channels())
        cout<< "Mark and binary matrices are different sizes"<<endl;
    
    // Confirm stack routines are working properly before running flood fill
    //testStack(binary,dataDir);
    //return 0;
    
    floodFill(binary, regions);
    imshow("Regions",regions);
    
    return 0;
}
