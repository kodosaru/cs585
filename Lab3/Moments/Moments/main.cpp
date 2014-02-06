//
//  main.cpp
//  Moments
//
//  Created by Don Johnson on 2/5/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <sys/stat.h>
#include <dirent.h>
#include <iomanip>

using namespace cv;
using namespace std;

int main(int argc, const char * argv[])
{
    string dataDir="/Users/donj/workspace/cs585/Lab3/Data/";
    Mat image=imread(dataDir+"gradient_grey_gs_vsmall.png",CV_LOAD_IMAGE_GRAYSCALE);
    Moments m=moments(image, false);
    cout.setf(ios_base::fixed);
    cout.precision(0);
    // mji where j is the x-moment
    cout<<"m00= "<<m.m00<<endl;
    cout<<"m10= "<<m.m10<<endl;
    cout<<"m01= "<<m.m01<<endl;
    cout<<"m20= "<<m.m20<<endl;
    cout<<"m02= "<<m.m02<<endl;
    cout<<"m21= "<<m.m21<<endl;
    cout<<"m12= "<<m.m12<<endl;
    cout<<"m30= "<<m.m30<<endl;
    cout<<"m03= "<<m.m03<<endl;

    return 0;
}

