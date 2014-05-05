//
//  main.cpp
//  PointClick
//
//  Created by Don Johnson on 3/16/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//


#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;

Point2f point;
bool addRemovePt = false;

static void onMouse( int event, int x, int y, int /*flags*/, void* /*param*/ )
{
    if( event == CV_EVENT_LBUTTONDOWN )
    {
        cout << "Mouse callback function" << endl;
        point = Point2f((float)x, (float)y);
        addRemovePt = true;
    }
}

int main( int argc, char** argv )
{
    VideoCapture cap(0);
    TermCriteria termcrit(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03);
    
    const int MAX_COUNT = 500;
    
    if( !cap.isOpened() )
    {
        cout << "Could not initialize capturing...\n";
        return 0;
    }
    
    namedWindow( "PointClick Demo", 1 );
    setMouseCallback( "PointClick Demo", onMouse, 0 );
    
    Mat image;
    vector<Point2f> points;
    
    for(;;)
    {
        Mat frame;
        cap >> frame;
        if( frame.empty() )
            break;
        
        frame.copyTo(image);
        
        if( !points.empty() )
        {

            size_t i;
            for( i = 0; i < points.size(); i++ )
            {
                circle( image, points[i], 3, Scalar(0,255,0), -1, 8);
            }
        }
        
        if( addRemovePt && points.size() < (size_t)MAX_COUNT )
        {
            points.push_back(point);
            addRemovePt = false;
        }
        
        imshow("PointClick Demo", image);
        
        char c = (char)waitKey(10);
        if( c == 27 )
            break;
        switch( c )
        {
            case 'c':
                points.clear();
                break;
        }
    }
    
    return 0;
}
