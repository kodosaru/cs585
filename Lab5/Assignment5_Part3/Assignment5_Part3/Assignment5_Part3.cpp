// Create a spring-time collage of images to encourage spring to come sooner
// Here is a basic skeleton. Fill it in any way that suits you to create your composition
// Use any pieces of any labs and homeworks that you need

#include "functions.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <string>
#include <vector>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace cv;
using namespace std;

//Suggestions only
Mat getRotationMatrix(double degrees);

//Suggestion only
vector<Mat> originals;
Mat result;

//For reference
//http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html
int main(int argc, char* argv[])
{
    String dataDir="/Users/donj/workspace/cs585/Lab5/Data/";
    String collageSourceDir="/Users/donj/workspace/cs585/Lab5/collageSource/";
    String backgroundImage="Clear_sky_over_Riga_2008.jpg";
    vector<string> fileNames;
    cout << "File count: " << listFiles(collageSourceDir, fileNames) << endl;
    
    int gridx=2, gridy=2;
    int maxDim;
    Mat result = imread(dataDir+backgroundImage);
    int minDimResult = result.cols > result.rows ? result.rows : result.cols;
    for(int i=0; i < fileNames.size(); i++)
    {
        cout << fileNames[i] << endl;
        originals.push_back(imread(collageSourceDir+fileNames[i]));
        maxDim = originals[i].cols > originals[i].rows ? originals[i].cols : originals[i].rows;
        if ( originals.size() == gridx*gridy )
            break;
    }

    
    //result.create(750, 750, CV_8UC3);
    tile(originals,result,gridx,gridy);

    namedWindow("Result", 1);

    while( 1 == 1)
    {
        imshow("Result", result);
        char key = waitKey(33);
        if(key == 'q')
        {
            break;
        }
        if(key == ' ')
        {
            imwrite("Assignment5_Output.png", result);
        }
    }
    return 0;
}

