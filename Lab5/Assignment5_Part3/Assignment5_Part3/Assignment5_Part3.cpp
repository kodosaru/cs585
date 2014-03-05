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
    String backgroundImage="sky2.png";
    vector<string> fileNames;
    cout << "File count: " << listFiles(collageSourceDir, fileNames) << endl;
    
    int gridx=3, gridy=3;
    result = imread(dataDir+backgroundImage);
    resize(result,result,Size(0,0),0.8,0.8,INTER_LINEAR);

    Mat collageImage;
    for(int i=0; i < fileNames.size(); i++)
    {
        cout << fileNames[i] << endl;
        collageImage = imread(collageSourceDir+fileNames[i]);
        originals.push_back(collageImage);

    }

    if ( originals.size() < gridx*gridy )
    {
        cout << "Insufficient images to tile background" << endl;
        return 0;
    }

    mosaic(originals, result, 9);
    resize(result,result,Size(result.cols/2,result.rows/2));
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
            imwrite(dataDir+"Assignment5_Output_large.png", result);
            resize(result,result,Size(0,0),750.0/result.cols,750.0/result.cols);
            imwrite(dataDir+"Assignment5_Output.png", result);
        }
    }
    return 0;
}

