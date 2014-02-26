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


void resizeImage(Point sizeBackground, float factor, Mat& image)
{
    float imageAspect = image.cols/(float)image.rows;
    float ratioWidth = image.cols/(float)sizeBackground.x;
    float ratioHeight = image.rows/(float)sizeBackground.y;
    float resizeFactor, newWidth, newHeight;

    if(ratioWidth>ratioHeight)
    {
        resizeFactor = sizeBackground.x/(image.cols * factor);
        newWidth = resizeFactor * image.cols;
        newHeight = newWidth / imageAspect;
    }
    else
    {
        resizeFactor = sizeBackground.y/(image.rows * factor);
        newHeight = resizeFactor * image.rows;
        newWidth = newHeight * imageAspect;
    }
    resize(image, image, Size(newWidth,newHeight));
}

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
    Mat result = imread(dataDir+backgroundImage);
    Mat collageImage;
    Point sizeResult(result.rows,result.cols);
    for(int i=0; i < fileNames.size(); i++)
    {
        cout << fileNames[i] << endl;
        collageImage = imread(collageSourceDir+fileNames[i]);
        resizeImage(sizeResult, 2.0, collageImage);
        originals.push_back(collageImage);
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

