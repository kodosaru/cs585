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
Mat originalEdges, edges;
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
int erosion_size = 0;
int dilation_size = 0;
int const max_kernel_size = 21;
bool morph = false;
char morphType = 0;


int main(int argc, char* argv[])
{
    String dataDir="/Users/donj/workspace/cs585/Lab2/Mac/Assignment2/Data/";
    if (argc < 2)
    {
        cout<<"Usage: Lab2 imageName"<<endl;
        return 0;
    }
    outputCounter=1;
    edgeMode = EDGEMODE_GRADIENT;
    
    //load in the image and convert it to gray scale
    readImageAndConvert(dataDir + argv[1], original);
    if(original.empty())
    {
        cout<<"Unable to open the image file: "<<argv[1]<<endl;
        return 0;
    }

    initializeData();
    createWindows();
    onThresholdTrackbar(threshSlider, NULL);
    
    //display the images and wait for 33 milliseconds in a loop, to
    //allow us to refresh the displayed image when we click at locations in the image
    while(1)
    {
        imshow("Edges", edges);
        imshow("Image View", displayImage);
        imshow("Mask View", maskImage);
        //imshow("Gradient Magnitude", displayGradientMagnitude);

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
            //refineEdgesMorphological(edges, key);
            // Adjust morphological settings with track bar
            
            morphType = key;
            morph = true;
        }
        
    }
    
    return 0;
}



