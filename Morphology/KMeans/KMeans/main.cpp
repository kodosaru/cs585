//
//  main.cpp
//  KMeans
//
//  Created by Don Johnson on 4/18/14.
//
//http://stackoverflow.com/questions/9575652/opencv-using-k-means-to-posterize-an-image

#include "opencv/cv.h"
#include "opencv/highgui.h"

#include <iostream>

using namespace cv;
using namespace std;

int main2(int argc, char* argv[]) {
    string dataDir="/Users/donj/workspace/cs585/Morphology/Data/";
    Mat src;
    
    src = imread(dataDir+argv[1]);
    imshow("original", src);
    
    blur(src, src, Size(15,15));
    imshow("blurred", src);
    
    Mat p = Mat::zeros(src.cols*src.rows, 5, CV_32F);
    Mat bestLabels, centers, clustered;
    vector<Mat> bgr;
    cout<<"Channels: "<<src.channels()<<endl;
    cv::split(src, bgr);
    // i think there is a better way to split pixel bgr color
    for(int i=0; i<src.cols*src.rows; i++) {
        p.at<float>(i,0) = (i/src.cols) / src.rows;
        p.at<float>(i,1) = (i%src.cols) / src.cols;
        p.at<float>(i,2) = bgr[0].data[i] / 255.0;
        p.at<float>(i,3) = bgr[1].data[i] / 255.0;
        p.at<float>(i,4) = bgr[2].data[i] / 255.0;
    }
    
    int K = 10;
    cv::kmeans(p, K, bestLabels,
               TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0),
               3, KMEANS_PP_CENTERS, centers);
    
    int colors[K];
    for(int i=0; i<K; i++) {
        colors[i] = 255/(i+1);
    }
    // i think there is a better way to do this mayebe some Mat::reshape?
    clustered = Mat(src.rows, src.cols, CV_32F);
    for(int i=0; i<src.cols*src.rows; i++) {
        clustered.at<float>(i/src.cols, i%src.cols) = (float)(colors[bestLabels.at<int>(0,i)]);
        //      cout << bestLabels.at<int>(0,i) << " " <<
        //              colors[bestLabels.at<int>(0,i)] << " " <<
        //              clustered.at<float>(i/src.cols, i%src.cols) << " " <<
        //              endl;
    }
    
    clustered.convertTo(clustered, CV_8U);
    imshow("clustered", clustered);
    imwrite(dataDir+"cluster.png", clustered);
    
    waitKey();
    return 0;
}