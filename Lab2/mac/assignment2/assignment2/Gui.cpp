#include "Assignment2.hpp"

void onSmoothTrackbar(int value, void* data)
{
    original.copyTo(image);
    smoothImage(image, value);
    //gradientSobel(image, dX, dY);
    markImageForDisplay(image, displayImage, maskImage);
    
    Mat dX, dY;
    gradientSobel(image, dX, dY, gradientMagnitude);
    convertGradientImageForDisplay(gradientMagnitude, displayGradientMagnitude);
    
    onThresholdTrackbar(threshSlider, NULL);
}

void onThresholdTrackbar(int value, void* data)
{
    //re-initialize the edge map and mask?
    edges = Mat::zeros(edges.rows, edges.cols, CV_8U);
    if(edgeMode == EDGEMODE_GRADIENT)
    {
        findEdgesGradientMagnitude(image, edges, threshSlider);
    }
    if(edgeMode == EDGEMODE_CANNY)
    {
        findEdgesCanny(image, edges, threshSlider);
        edges = edges/2;
    }
    edges.copyTo(maskImage);
    markImageForDisplay(image, displayImage, maskImage);
}
