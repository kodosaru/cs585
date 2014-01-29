#include "Assignment2.hpp"

static void onClickCallback( int event, int x, int y, int q, void* data)
{
    if( event != EVENT_LBUTTONDOWN )
        return;
    
    //Flood fill region bounded by edges
    fillRegionBoundedByEdges(edges, maskImage, x, y);
    
    markImageForDisplay(image, displayImage, maskImage);
}


void onSmoothTrackbar(int nvalue, void* data)
{
    original.copyTo(image);
    // Value not dropping to zero so subtract 1
    nvalue--;
    nvalue = nvalue < 1 ? 0 : nvalue;
    // Allow .5 sigma values
    double value = (double)nvalue / 2.0;
    cout << "Smoothing Sigma: " << value << endl;
    if(value > 0)
    {
        smoothImage(image, value);
    }
    //gradientSobel(image, dX, dY);
    markImageForDisplay(image, displayImage, maskImage);
    
    //Mat dX, dY;
    //gradientSobel(image, dX, dY, gradientMagnitude);
    //convertGradientImageForDisplay(gradientMagnitude, displayGradientMagnitude);
    
    onThresholdTrackbar(threshSlider, NULL);
}

void onThresholdTrackbar(int value, void* data)
{
    //re-initialize the edge map and mask?
    edges = Mat::zeros(edges.rows, edges.cols, CV_8U);
    cout << "Threshold: " << value << endl;
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

void createWindows()
{
    // Create a new windows
    namedWindow("Control Window",0);
    namedWindow( "Edges", 1);
    namedWindow( "Image View", 1);
    //namedWindow( "Mask View", 1);
    //namedWindow( "Test", 1);
    
    //namedWindow( "Gradient Magnitude", 1);
    int ydisplacement = 160;
    int offset = 20;

    imshow("Edges", edges);
    moveWindow("Edges", 1 * offset, ydisplacement + 1 * offset);
    imshow("Image View", displayImage);
    moveWindow("Image View", 2 * offset, ydisplacement + 2 * offset);
    imshow("Mask View", maskImage);
    moveWindow("Mask View", 3 * offset, ydisplacement + 3 * offset);
    //imshow("Gradient Magnitude", displayGradientMagnitude);
    //moveWindow("Gradient Magnitude", ydisplacement + 4 * offset, 4 * offset);
    
    //attach a mouse click callback to the window
    setMouseCallback("Image View", onClickCallback, NULL);
    createTrackbar( "Smoothing", "Control Window", &smoothSlider, smoothSliderMax, onSmoothTrackbar );
    createTrackbar( "Threshold", "Control Window", &threshSlider, threshSliderMax, onThresholdTrackbar );
    createTrackbar( "Erosion", "Control Window", &erosion_size, max_kernel_size, Erosion );
    createTrackbar( "Dilation", "Control Window", &dilation_size, max_kernel_size, Dilation );

}