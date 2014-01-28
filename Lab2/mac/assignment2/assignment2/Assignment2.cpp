// CS585 Assignment 2
// Edge Detection in OpenCV
//
//
// Copyright 2014 Diane H. Theriault
//
//
#include "Assignment2.hpp"

//Global variables are evil, but easy
Mat original, image, displayImage;
Mat maskImage;
Mat edges;
Mat gradientMagnitude, displayGradientMagnitude;
int outputCounter;
int edgeMode; 
//while we are engaging in bad software practices, let's try some magic numbers
const int EDGEMODE_GRADIENT=0;
const int EDGEMODE_LAPLACIAN=1;
const int EDGEMODE_CANNY=2;

int smoothSlider;
int smoothSliderMax = 20;
int threshSlider;
int threshSliderMax = 256;


//Given: the callback for the click
static void onClickCallback( int event, int x, int y, int q, void* data);

int main(int argc, char* argv[])
{
    String inputDir="/Users/donj/workspace/cs585/Lab2/Mac/Assignment2/Data/";
    if (argc < 2)
    {
        cout<<"Usage: Lab2 imageName"<<endl;
        return 0;
    }
    outputCounter=1;
    edgeMode = EDGEMODE_GRADIENT;

    //load in the image and convert it to gray scale
    readImageAndConvert(inputDir + argv[1], original);
    if(original.empty())
    {
        cout<<"Unable to open the image file: "<<argv[1]<<endl;
        return 0;
    }
    original.copyTo(image);

    //initialize the versions of the image that we will use for displaying some things
    displayImage.create(image.rows, image.cols, CV_8UC3);
    maskImage=Mat::zeros(image.rows, image.cols, CV_8UC1);
    markImageForDisplay(image, displayImage, maskImage);

    Mat dX, dY;
    gradientMagnitude.create(image.rows, image.cols, CV_32FC1);
    displayGradientMagnitude.create(image.rows, image.cols, CV_8UC1);
    gradientSobel(image, dX, dY, gradientMagnitude);
    convertGradientImageForDisplay(gradientMagnitude, displayGradientMagnitude);

    edges.create(image.rows, image.cols, CV_8UC1);

    // Create a new windows
    namedWindow("Control Window",0);
    namedWindow( "Image View", 1);
    namedWindow( "Edges", 1);
    namedWindow( "Gradient Magnitude", 1);

    //attach a mouse click callback to the window
    setMouseCallback("Image View", onClickCallback, NULL);
    createTrackbar( "Smoothing", "Control Window", &smoothSlider, smoothSliderMax, onSmoothTrackbar );
    createTrackbar( "Threshold", "Control Window", &threshSlider, threshSliderMax, onThresholdTrackbar );

    //display the images and wait for 33 milliseconds in a loop, to 
    //allow us to refresh the displayed image when we click at locations in the image
    while(1)
    {
       //imshow("Image View", displayImage);
       //imshow("Mask View", maskImage);
       imshow("Gradient Magnitude", displayGradientMagnitude);
       //imshow("Edges", edges);
       char key=waitKey(33);
       if(key == 'Q' || key == 'q')
       {
           break;
       }
       if(key == 'S' || key == 's')
       {
           char filename1[512];
           char filename2[512];
           sprintf(filename1, "filledRegion_%03d.png", outputCounter);
           imwrite(filename1, displayImage);
           sprintf(filename2, "edges_%03d.png", outputCounter);
           imwrite(filename2, edges);

           cout<<"Images Saved: "<<filename1<<" "<<filename2<<endl;
           outputCounter++;
       }
       if(key == 'g')
       {
           edgeMode = EDGEMODE_GRADIENT;
           onThresholdTrackbar(threshSlider, NULL);
       }
       if(key == 'c')
       {
           edgeMode = EDGEMODE_CANNY;
           onThresholdTrackbar(threshSlider, NULL);
       }
       if(key == 'e' || key == 'd')
       {
            refineEdgesMorphological(edges, key);
       }

    }

    return 0;
}

static void onClickCallback( int event, int x, int y, int q, void* data)
{
    if( event != EVENT_LBUTTONDOWN )
        return;
    
    //Flood fill region bounded by edges
    fillRegionBoundedByEdges(edges, maskImage, x, y);
    
    markImageForDisplay(image, displayImage, maskImage);
}

