// Assignment 6, Do the mosaic creation pipeline for three images

#include "stdafx.h"
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

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
        vector<Mat> originals;
    Mat result;

    for(int i=1; i<argc; i++)
    {
        originals.push_back(imread(argv[i]));
    }
    Stitcher stitcher = Stitcher::createDefault(false);
    Stitcher::Status status = stitcher.stitch(originals, result);
    
    imshow("result", result);
    waitKey(0);

}
