//
//  FloodFillMethods.cpp
//  FloodFill
//
//  Created by Don Johnson on 4/24/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#include "FloodFillMethods.h"
#include "Archive.hpp"

using namespace std;
using namespace cv;

// Implementation of Diane's flood fill algorithm below
// Run this algorithm against every pixel in an image to mark all regions
//• Depth-first Traversal(self)!
//      Stack.push(self)!
//–     While(!stack.empty())!
//•         node = stack.pop()!
//•         For each neighbor!
//–             If not marked!
//»                 Mark neighbor!
//»                 Stack.push(neighbor)!

// Begin flood fill algorithm (depthFirstTransversal)
void floodFill(Mat& image, Mat& regions, unsigned short& nRegions, vector<vector<PIXEL>*>& regionLists)
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
    regions=Scalar(0);
    Stack<PIXEL> s((int)image.rows*image.cols);
    nRegions=0;
    unsigned long nRegionsPixels;
    PIXEL imagePixel, neighborPixel, regionsPixel;
    
    for(unsigned short i=0;i<USHRT_MAX;i++)
        regionLists[i]=new vector<PIXEL>;
    
    for(int y=0;y<image.rows;y++)
    {
        //cout<<"Row: "<<y<<endl;
        
        // Retrieve pixel from image
        for(int x=0;x<image.cols;x++)
        {
            imagePixel.pt=Point(x,y);
            getPixel_8UC1(image, imagePixel);
            
            // Push the pixel on to stack to start process of neighbor check
            regionsPixel.pt=imagePixel.pt;
            getPixel_16UC1(regions, regionsPixel);
            if(regionsPixel.val[0] > 0)
            {
                //cout<<"Pixel("<<imagePixel.pt.x<<","<<imagePixel.pt.y<<") already assigned to region "<<regionsPixel.val[0]<<endl;
            }
            else
            {
                s.push(imagePixel);
                
                // Increment region counter
                nRegions++;
                nRegionsPixels=0;
                
                cout<<"Starting region "<<nRegions<<" transversal"<<endl;
                
                // Define the regions map pixel value and set
                nRegionsPixels++;
                imagePixel.val[0]=nRegions;
                setPixel_16UC1(imagePixel,regions);
                regionLists[nRegions-1]->push_back(imagePixel);
                //printf("Mark pixel(%d,%d)=%0.0f\n",imagePixel.pt.x,imagePixel.pt.y,imagePixel.val[0]);
                
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
                                        nRegionsPixels++;
                                        neighborPixel.val[0]=nRegions;
                                        setPixel_16UC1(neighborPixel,regions);
                                        regionLists[nRegions-1]->push_back(neighborPixel);
                                        //printf("Mark pixel(%d,%d)=%0.0f\n",neighborPixel.pt.x,neighborPixel.pt.y,neighborPixel.val[0]);
                                    }
                                }
                            } // End in bounds check
                        } // End of neighborhood row transversal
                    } // End neighborhood column transversal
                } // End of region transversal
                
                cout<<"Ending region "<<nRegions<<" count:"<<nRegionsPixels<<endl;
                
            } // End of check if push new pixel or skip if already in other region
        } // End of image row transversal
    } // End of image column transversal
} // End of flood fill function

void adjustContrastBrightness(Mat& image, Mat& new_image, int beta, double alpha)
{
    int channels=image.channels();
    
    /// Do the operation new_image(i,j) = alpha*image(i,j) + beta
    if(channels==1)
    {
        for( int y = 0; y < image.rows; y++ )
        {
            for( int x = 0; x < image.cols; x++ )
            {
                new_image.at<uchar>(y,x) = alpha * image.at<uchar>(y,x) + beta;
            }
        }
    }
    else if (channels==3)
    {
        for( int y = 0; y < image.rows; y++ )
        {
            for( int x = 0; x < image.cols; x++ )
            {
                for( int c = 0; c < channels; c++ )
                {
                    new_image.at<cv::Vec3b>(y,x)[c] = alpha * image.at<cv::Vec3b>(y,x)[c] + beta;
                }
            }
        }
        
    }
    else
    {
        cout<<"Unable to adjust contrast for this type of image"<<endl;
    }
}

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