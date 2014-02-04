// Assignment3_Part1.cpp : Defines the entry point for the console application.
//

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sys/stat.h>

using namespace cv;
using namespace std;

//Make the original a global variable for the sake of event handling
Mat originalImage;

bool FileExist( const string& Name );

int main(int argc, char* argv[])
{
    String dataDir="/Users/donj/workspace/cs585/Lab3/Data/";
    String path;

    //moving and scaling an outline

    if(argc <= 1)
    {
        cout<<"Please provide a filename of an image"<<endl;
        return 0;
    }
    else {
            path = dataDir+argv[1];
    }

    if(!FileExist(path)){
        cout << "File " << path << " does not exist" << endl;
        return 1;
    }
    originalImage = imread(path);
    Mat displayImage(originalImage.rows, originalImage.cols, CV_8UC3);
    originalImage.copyTo(displayImage);
    vector<Point> outline;
    int redThreshold = 190;

}

bool FileExist( const string& Name )
{
#ifdef OS_WINDOWS
    struct _stat buf;
    int Result = _stat( Name.c_str(), &buf );
#else
    struct stat buf;
    int Result = stat( Name.c_str(), &buf );
#endif
    return Result == 0;
}
