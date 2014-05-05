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
    //String dataDir="/Users/donj/workspace/cs585/Lab2/Mac/Assignment2/Data/";
    String dataDir="/Users/donj/workspace/cs585/Lab2/Mac/Assignment2/Data/camoEggsPyramid/";
    
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
        //imshow("Mask View", maskImage);
        //imshow("Gradient Magnitude", displayGradientMagnitude);

        char key=waitKey(33);
        if(key == 'Q' || key == 'q')
        {
            break;
        }
        if(key == 'S' || key == 's')
        {

            //String filename1 = dataDir + "SoloYellowEgg-Gradient-edges.png";
            //String filename2 = dataDir + "SoloYellowEgg-Gradient-result.png";
            //String filename1 = dataDir + "SoloYellowEgg-Canny-edges.png";
            //String filename2 = dataDir + "SoloYellowEgg-Canny-result.png";
            //String filename1 = dataDir + "redEgg-edges.png";
            //String filename2 = dataDir + "redEgg-result.png";
            //String filename1 = dataDir + "blueEgg-edges.png";
            //String filename2 = dataDir + "blueEgg-result.png";
            //String filename1 = dataDir + "greenEgg-edges.png";
            //String filename2 = dataDir + "greenEgg-result.png";
            //String filename1 = dataDir + "cube-edges.png";
            //String filename2 = dataDir + "cube-result.png";
            //String filename1 = dataDir + "cube-canny-dilation-edges.png";
            //String filename2 = dataDir + "cube-canny-dilation-result.png";
            //String filename1 = dataDir + "camoEggsOnPaper-canny-dilation-edges.png";
            //String filename2 = dataDir + "camoEggsOnPaper-canny-dilation-result.png";
            //String filename1 = dataDir + "rainbowEasy-edges.png";
            //String filename2 = dataDir + "rainbowEasy-result.png";
            //String filename1 = dataDir + "rainbowHard-before-edges.png";
            //String filename2 = dataDir + "rainbowHard-before-result.png";
            //String filename1 = dataDir + "rainbowHard-after-edges.png";
            //String filename2 = dataDir + "rainbowHard-after-result.png";
            String filename1 = dataDir + "camoEggsPyramid-edges.png";
            String filename2 = dataDir + "camoEggsPyramid-result.png";
           
            imwrite(filename1, edges);
            imwrite(filename2, displayImage);
            
            cout << "Image Saved: " << filename1 << endl;
            cout << "Image Saved: " << filename2 << endl;
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



