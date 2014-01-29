#include "Assignment2.hpp"

void fillRegionBoundedByEdges(Mat& edges, Mat& mask, int seedX, int seedY)
{
    edges.copyTo(mask);
    //int channels = mask.channels();
    Point seedPoint(seedX, seedY);
    //http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#floodfill
    //floodFill(edges, mask, seedPoint, Scalar(128), 0, Scalar(), Scalar(), 4 );
     uchar fillValue = 255;
     //Point seed(4,4);
    copyMakeBorder(mask, mask, 1, 1, 1, 1, BORDER_REPLICATE);
    floodFill(edges, mask, seedPoint, Scalar(fillValue), 0, Scalar(), Scalar(), 4 | FLOODFILL_MASK_ONLY | (fillValue << 8));

}

void refineEdgesMorphological(Mat& edges, char erodeOrDilate)
{
    //http://docs.opencv.org/doc/tutorials/imgproc/erosion_dilatation/erosion_dilatation.html
    
    if(erodeOrDilate == 'e')
    {
    }
    else if(erodeOrDilate == 'd')
    {
    }
}


void findEdgesCanny(Mat& image, Mat& edges, double thresh)
{
    //http://docs.opencv.org/doc/tutorials/imgproc/imgtrans/canny_detector/canny_detector.html
    // Input to this function is already blurred
    Scalar mean,stddev;
    meanStdDev( image, mean, stddev);

    int threshholdRatio = 3;
    //int lowThreshold = (int)0.66 * mean;
    int lowThreshold = thresh;
    //int highThreshold = (int)1.33 * mean;
    int highThreshold = threshholdRatio * lowThreshold;
    int kernelSize = 3;

    cout << "Canny Threshold Low: " << lowThreshold << "  High: " << lowThreshold * threshholdRatio << endl;
    Canny( image, edges, lowThreshold, highThreshold, kernelSize);
}


void findEdgesGradientMagnitude(Mat& image, Mat& edges, double thresh)
{
    Mat dX, dY;
    
    //allocate objects for holding derivatives and gradients
    dX.create(image.rows, image.cols, CV_32F);
    dY.create(image.rows, image.cols, CV_32F);
    gradientMagnitude.create(image.rows, image.cols, CV_32F);
    gradientSobel(image, dX, dY, gradientMagnitude);
    //threshold(gradientMagnitude,edges,thresh,255,0);
    /* Note from teacher: I came across a problem regarding flood fill, it was not happening when I was using the sliders to threshold and smooth.  It turned out that the function threshold changes the type of the second argument, and therefore it should be converted back. */
    cout << "Gradient Threshold: " << thresh << endl;
    threshold(gradientMagnitude, gradientMagnitude, thresh, 128, THRESH_BINARY);
    gradientMagnitude.convertTo(edges, CV_8UC1);
    //gradientMagnitude.copyTo(edges);
}


void gradientSobel(Mat& image, Mat& dX, Mat& dY, Mat& magnitude)
{
    //http://docs.opencv.org/modules/imgproc/doc/filtering.html?highlight=sobel#sobel
    //calculate the gradients with OpenCV Sobel() function
    Sobel( image, dX, CV_32F, 1, 0, 3, 1, 0, BORDER_DEFAULT );
    Sobel( image, dY, CV_32F, 0, 1, 3, 1, 0, BORDER_DEFAULT );
    
    //to compute gradient magnitude:
    // http://docs.opencv.org/modules/core/doc/operations_on_arrays.html?highlight=multiply#multiply
    // http://docs.opencv.org/modules/core/doc/operations_on_arrays.html?highlight=add#add
    // http://docs.opencv.org/modules/core/doc/operations_on_arrays.html?highlight=sqrt#sqrt
    
    Mat temp(dX.rows, dY.cols, CV_32F);
    multiply(dX, dX, temp);
    temp.copyTo(magnitude);
    multiply(dY, dY, temp);
    add(temp, magnitude, magnitude);
    sqrt(magnitude, magnitude);
}


void smoothImage(Mat& image, double sigma)
{
    if(sigma <= 0.0)
    {
        return;
    }
    //smooth the image
    // This is another example of a convolution / filtering operation, this time with a
    // Gaussian kernel. You could also use all ones to get the mean of the pixels in the image
    GaussianBlur(image, image, Size(0,0), sigma, sigma, BORDER_DEFAULT);
}

bool convertGradientImageForDisplay(Mat& input, Mat& output)
{
    if(input.empty())
    {
        return false;
    }
    if(output.empty() || input.rows != output.rows || input.cols != output.cols)
    {
        return false;
    }
    
    Mat temp; //make a copy so we don't change the input image
    input.copyTo(temp);
    //convert to range 0 - 255
    double minVal, maxVal;
    minMaxLoc(temp, &minVal, &maxVal);
    
    //keep zero centered at 128
    if(minVal < 0)
    {
        maxVal = max(abs(minVal), abs(maxVal));
        minVal = -maxVal;
    }
    
    //scale image into range [0 255] and convert from float to unsigned char
    temp = (temp-minVal) / (maxVal-minVal)*255;
    temp.convertTo(output, CV_8UC1);
    return true;
}

bool markImageForDisplay(Mat& gray, Mat& output, Mat& mask)
{
    //imshow("Test",mask);
    
    //duplicate gray image into three channels and place one red pixel
    vector<Mat> channels;
    for(int i=0; i<3; i++)
    {
        channels.push_back(gray);
    }
    merge(channels, output);
    
    //imshow("Test",output);
    
    //anywhere that is marked in the mask image, suppress the green and blue
    //channels so that the region will be highlighted red
    int numchannels = mask.channels();
    for(int row=0; row<output.rows; row++)
    {
        unsigned char* maskPtr = mask.ptr<unsigned char>(row);
        unsigned char* imgPtr = output.ptr<unsigned char>(row);
        for(int col=0; col<output.cols; col++)
        {
            if(maskPtr[col]==255)
            {
                imgPtr[col*3]=0;
                imgPtr[col*3+1]=0;
                imgPtr[col*3+2]=255;
            }
        }
    }
    //imshow("Test",output);
    return true;
}

bool readImageAndConvert(const string& filename, Mat& grayImage)
{
    //read image from file and convert to gray scale
    Mat image;
    image = imread(filename);
    if(image.empty())
    {
        return false;
    }
    cvtColor( image, grayImage, CV_RGB2GRAY );
    return !grayImage.empty();
}

void initializeData()
{
    original.copyTo(image);
    
    //initialize the versions of the image that we will use for displaying some things
    displayImage.create(image.rows, image.cols, CV_8UC3);
    maskImage=Mat::zeros(image.rows, image.cols, CV_8UC1);
    markImageForDisplay(image, displayImage, maskImage);
    
    //Mat dX, dY;
    //gradientMagnitude.create(image.rows, image.cols, CV_32FC1);
    //displayGradientMagnitude.create(image.rows, image.cols, CV_8UC1);
    //gradientSobel(image, dX, dY, gradientMagnitude);
    //convertGradientImageForDisplay(gradientMagnitude, displayGradientMagnitude);
    
    edges.create(image.rows, image.cols, CV_8UC1);
}

