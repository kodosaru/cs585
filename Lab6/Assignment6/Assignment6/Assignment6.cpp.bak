// Assignment 6, Do the mosaic creation pipeline for two images

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
#include <stdio.h>
#include "Functions.h"

using namespace cv;
using namespace std;

//http://docs.opencv.org/doc/tutorials/features2d/feature_homography/feature_homography.html
//http://docs.opencv.org/trunk/doc/py_tutorials/py_feature2d/py_orb/py_orb.html

int main(int argc, char* argv[])
{
    String dataDir="/Users/donj/workspace/cs585/Lab6/Data/";
    vector<Mat> img;

    int nImages=atoi(argv[1]);
    int nResults=4;
    vector<Mat> result(nResults);
    for(int i=0;i<nImages;i++)
    {
        img.push_back(imread(dataDir+argv[i+2]));
    }
    
    Size mosaicSize;
    Point2f mosaicOffset;
    Mat H;

    H = getHomography(img[0], img[1]);

    //mess with the color so we can see the seam
    cvtColor(img[0], img[0], CV_BGR2RGB);

    computeMosaicFootprint(img[0], img[1], H, mosaicSize, mosaicOffset);

    composeMosaic_v1(img[0], img[1], H, mosaicSize, mosaicOffset, result[0]);
    
    imwrite(dataDir+"Assignment6_Part1_Result1.png", result[0]);
    imshow("Result1", result[0]);
    waitKey(0);

    composeMosaic_v2(img[0], img[1], H, mosaicSize, mosaicOffset, result[1]);
    imwrite(dataDir+"Assignment6_Part1_Result2.png", result[1]);
    imshow("Result2", result[1]);
    waitKey(0);
    
    composeMosaic_v2B(img[0], img[1], H, mosaicSize, mosaicOffset, result[2]);
    imwrite(dataDir+"Assignment6_Part1_Result2b.png", result[2]);
    imshow("Result2b", result[2]);
    waitKey(0);
    
    H = getHomography(img[1], img[0]);
    computeMosaicFootprint(img[1], img[0], H, mosaicSize, mosaicOffset);
    composeMosaic_v1(img[1], img[0], H, mosaicSize, mosaicOffset, result[3]);
    
    imwrite(dataDir+"Assignment6_Part1_Result1_backwards.png", result[3]);
    imshow("Result1", result[3]);
    waitKey(0);
}
