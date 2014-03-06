// Lab3_Part2.cpp : Defines the entry point for the console application.
//

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <string>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

void onTrackbar(int value, void* data) {}

int main(int argc, char* argv[])
{
    String dataDir="/Users/donj/workspace/cs585/Lab6/Data/";

    if(argc < 3)
    {
        //example arguments: ruby.jpg basket.jpg
        cout<<"Please provide a filename of an exemplar image and a testing image"<<endl;
        return 0;
    }

    Mat displayImage;

    //read in the image
    Mat image1, image2;
    image1 = imread(dataDir+argv[1]);
    image2 = imread(dataDir+argv[2]);
    Mat grayImage1, grayImage2;
    cvtColor(image1, grayImage1, CV_BGR2GRAY);
    cvtColor(image2, grayImage2, CV_BGR2GRAY);

    SiftFeatureDetector detector;
    vector<KeyPoint> keypoints1;
    vector<KeyPoint> keypoints2;
    detector.detect(grayImage1, keypoints1);
    detector.detect(grayImage2, keypoints2);

    // http://docs.opencv.org/modules/features2d/doc/common_interfaces_of_descriptor_extractors.html
    Mat descriptors1;
    Mat descriptors2;
    detector.compute(image1, keypoints1, descriptors1);
    detector.compute(image2, keypoints2, descriptors2);

    //http://docs.opencv.org/modules/features2d/doc/common_interfaces_of_descriptor_matchers.html#bfmatcher
    BFMatcher matchmaker;
    vector<DMatch> matches;
    matchmaker.match(descriptors1, descriptors2, matches);

    int maxVal = 500;
    int threshold = 150;
    int prevThreshold=0;
    
    //namedWindow("Image Window", CV_WINDOW_NORMAL);
    namedWindow("Image Window", CV_WINDOW_AUTOSIZE);
    createTrackbar("Match Thresh", "Image Window", &threshold, maxVal);

    while(1 == 1)
    {
        if(threshold != prevThreshold)
        {
            vector<DMatch> finalMatches;
            for(int i=0; i<matches.size(); i++)
            {
                if (matches[i].distance < threshold)
                {
                    finalMatches.push_back(matches[i]);
                }
            }

            //http://docs.opencv.org/modules/features2d/doc/drawing_function_of_keypoints_and_matches.html?
            drawMatches(image1, keypoints1, image2, keypoints2, finalMatches, displayImage);
            prevThreshold = threshold;
        }

        Point imgSize(displayImage.cols,displayImage.rows);
        imgSize.x/=4;
        imgSize.y/=4;
        Mat smallDisplayImage;
        resize(displayImage, smallDisplayImage, imgSize);
        imshow("Image Window", smallDisplayImage);
        char key = waitKey(33);
        if(key == 'q')
        {
            break;
        }
        if(key == ' ')
        {
            imwrite(dataDir+"output.png", smallDisplayImage);
        }
    }
}



