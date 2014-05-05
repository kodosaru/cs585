// Assignment 6, Do the mosaic creation pipeline for three images

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include <string>
#include <vector>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <Functions.h>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    String dataDir="/Users/donj/workspace/cs585/Lab6/Data/";
    vector<Mat> img;
    
    int nImages=atoi(argv[1]);
    int nResults=2;
    vector<Mat> result(nResults);
    for(int i=0;i<nImages;i++)
    {
        img.push_back(imread(dataDir+argv[i+2]));
    }
    
    Size mosaicSize;
    Point2f mosaicOffset;
    Mat H;
    
    //mess with the color so we can see the seam
    cvtColor(img[1], img[1], CV_BGR2RGB);
    
    //Note: Source files are back in the Assignment6 folder: Functions.cpp - just linking in binaries
    H = getHomography(img[0], img[1]);
    computeMosaicFootprint(img[0], img[1], H, mosaicSize, mosaicOffset);
    composeMosaic_v1(img[0], img[1], H, mosaicSize, mosaicOffset, result[0]);
    
    imshow("Preliminary", result[0]);
    waitKey(0);
    
    H = getHomography(result[0], img[2]);
    
    computeMosaicFootprint(result[0], img[2], H, mosaicSize, mosaicOffset);
    
    composeMosaic_v1(result[0], img[2], H, mosaicSize, mosaicOffset, result[1]);
    
    imwrite(dataDir+"Assignment6_Part2.png", result[1]);
    imshow("Three Image Mosaic", result[1]);
    waitKey(0);
}
