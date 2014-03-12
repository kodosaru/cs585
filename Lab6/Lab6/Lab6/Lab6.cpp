// Lab 6: This demonstrates the use of the OpenCV image stitching module.
// (It takes a little while to run, even for just three images.)


#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"
#include <iostream>
#include <stdio.h>

//http://docs.opencv.org/modules/stitching/doc/introduction.html
//http://docs.opencv.org/modules/stitching/doc/high_level.html

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/stitching/stitcher.hpp"


#include <string>
#include <vector>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace cv;
using namespace std;



int main(int argc, char* argv[])
{
    String dataDir="/Users/donj/workspace/cs585/Lab6/Data/";
    vector<Mat> originals;
    Mat result(7800,6000,CV_8UC3);

    for(int i=1; i<argc; i++)
    {
        originals.push_back(imread(dataDir+argv[i]));
        resize(originals[i-1],originals[i-1],Size(0,0),0.4,0.4,INTER_LINEAR);
    }
    Stitcher stitcher = Stitcher::createDefault(false);
    Stitcher::Status status = stitcher.stitch(originals, result);
    
    imshow("Result", result);
    imwrite(dataDir+"Assignment6_Lab6_Optional.png", result);
    waitKey(0);
}

