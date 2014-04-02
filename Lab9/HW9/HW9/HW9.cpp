//CS585 HW9
//For this homework, we will try our hand at using template tracking

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
using namespace cv;
using namespace std;

#define CLAMP 30
#define ALPHA 1.2
#define BETA 1.0
#define GAMMA 1.35
#define DEBUG 0
#define DEBUG_CONT 0
#define DEBUG_CURVE 0
#define DEBUG_GRAD 0

vector<Point> contour;
bool bRecordContour=false;

//Some helper functions to implement things we've covered before
void subsampleContour(vector<Point>& contour, int subsample);
void onMouse( int event, int x, int y, int /*flags*/, void* /*param*/ );
void drawOutline(Mat& image, vector<Point>& outline);
void gradientSobel(Mat& image,  Mat& magnitude);


//Some helper functions to implement the basic equations from the paper
double computeCurvature(Point p1, Point p2, Point p3);
double computeDistance(Point p1, Point p2);
double computeContinuity(double meanDist, Point p1, Point p2);
void normalizeScore(Mat& score);
void normalizeImageScore(Mat& score, double clamp=30);
double computeMeanPointDistance(vector<Point>& contour);


//Your task: Fill in the definitions of evolvePoint
void evolvePoint(Mat& image, Point& out, const Point& now, const Point& before, const Point& after,
                 Mat& gradientMagnitude, double meanDist, double alpha, double beta, double gamma);
double evolveContour(vector<Point>& contour, Mat& gray, double alpha, double beta, double gamma);

// Test function
void fillContour();

void myMinMaxLoc(Mat& array, double& minVal, double& maxVal, Point& minPt, Point& maxPt)
{
    int cols=array.cols;
    int rows=array.rows;
    minVal=DBL_MAX;
    maxVal=-DBL_MAX;
    for(int x=0;x<cols;x++)
        for(int y=0;y<rows;y++)
        {
            if( array.at<float>(x,y) < minVal)
            {
                minVal = array.at<float>(x,y);
                minPt = Point(x,y);
            }
            if( array.at<float>(x,y) > maxVal)
            {
                maxVal = array.at<float>(x,y);
                maxPt = Point(x,y);
            }
        }
}

int main( int argc, char** argv )
{
    String dataDir = "/Users/donj/workspace/cs585/Lab9/Data/";
    Mat gray, display, original;
    if(argc < 2)
    {
        cout<<"Need an image filename"<<endl;
        return 0;
    }
    original = imread(dataDir+argv[1]);
    //convert to grayscale
    cvtColor(original, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, gray, Size(9,9), 3,3); // this was very helpful when the contour was getting stuck in seemingly untextured regions
    
    namedWindow("Snake Contours", 1);
    setMouseCallback("Snake Contours", onMouse, NULL);
    
    bool bRecording=false;
    bool bEvolving = false;
    int currentFrame = 0;
    char filename[128];
    
    //fillContour();
    while(true)
    {
        
        if(bEvolving)
        {
            if(DEBUG)
                for(int i=0;i<contour.size();i++)
                    printf("Contour Point before (%d,%d)\n",contour[i].x,contour[i].y);
            double avgChange = evolveContour(contour, gray, ALPHA, BETA, GAMMA);
            if(DEBUG)
            {
                for(int i=0;i<contour.size();i++)
                    printf("Contour Point after (%d,%d)\n",contour[i].x,contour[i].y);
                printf("Avg change: %0.2f\n",avgChange);
            }
        }
        
        original.copyTo(display);
        drawOutline(display, contour);
        imshow("Snake Contours", display);
        
        //save the finished image if necessary
        if(bRecording)
        {
            sprintf(filename, "result_%04d.jpg", currentFrame);
            imwrite(filename, display);
            currentFrame++;
        }
        
        char c = (char)waitKey(60);
        if( c == 'q' )
        {
            break;
        }
        if( c == ' ')
        {
            bRecording = !bRecording;
        }
        if(c == 'e')
        {
            bEvolving = !bEvolving;
        }
    }
    
    return 0;
}



//Required: Given a point in the contour and some supporting information, evaluate
//the objective function in a neighborhood surrounding the point and choose a new
//location for the point
//
// Point out -- the new location of the point of interest
// Point now -- the beginning value of the point of interest
// Point before, after -- the points in the contour before and after the point of interest
// Mat gradientMagnitude -- the gradient magnitude of the patch surrounding the point
// double meanDist -- the average distance between points on the contour
// double alpha, beta, gamma -- the regularization parameters
void evolvePoint(Mat& image, Point& out, const Point& now, const Point& before, const Point& after, Mat& gradientMagnitude, double meanDist, double alpha, double beta, double gamma)
{
    Mat continuity(3,3,CV_32FC1);
    Mat curvature(3,3,CV_32FC1);
    Mat imageScore(3,3, CV_32FC1);
    Mat totalScore(3,3, CV_32FC1);
    double minVal, maxVal;
    Point minPt, maxPt;
    int dx,dy;
    
    // Here, fill in the code to visit each pixel in a 3 x 3 neighborhood and compute the value of the objective function
    // vi=now, vi-1=before, vi+1=after
    // Compute continuity and curvature energy terms
    for(dx=-1; dx<=1; dx++)
        for(dy=-1; dy<=1; dy++)
        {
            if(DEBUG && (now.x+dx < 0 || now.y+dy < 0 || now.x+dx > image.cols - 2 || now.y+dy > image.rows - 2))
                printf("Kernel cell out of bounds at (%d,%d)\n",now.x, now.y);
            continuity.at<float>(dx+1,dy+1) = computeContinuity(meanDist, Point(now.x+dx,now.y+dy), before);
            curvature.at<float>(dx+1,dy+1) = computeCurvature(before, Point(now.x+dx,now.y+dy), after);
            if(DEBUG_CONT)
                printf("Cont Mean: %0.2f now: (%d,%d) before: (%d,%d): (%d,%d) %0.6f\n",meanDist,now.x+dx,now.y+dy,before.x,before.y,dx+1,dy+1,continuity.at<float>(dx+1,dy+1));
            if(DEBUG_CURVE)
                printf("Curve (%d,%d) now (%d,%d) after (%d,%d): %0.2f\n",before.x,before.y,now.x+dx,now.y+dy,after.x,after.y,curvature.at<float>(dx+1,dy+1));
        }
    
    // Normalize energy terms
    // Continuity
    normalizeScore(continuity);
    if(DEBUG_CONT)
    {
        for(dx=-1; dx<=1; dx++)
            for(dy=-1; dy<=1; dy++)
            {
                printf("Cont normal Mean: %0.2f now: (%d,%d) before: (%d,%d): (%d,%d) %0.6f\n",meanDist,now.x+dx,now.y+dy,before.x,before.y,dx+1,dy+1,continuity.at<float>(dx+1,dy+1));
            }
        myMinMaxLoc(continuity, minVal, maxVal, minPt, maxPt);
        printf("My continuity min at (%d,%d): %0.2f and max at (%d,%d): %0.2f\n",minPt.x,minPt.y,minVal,
               maxPt.x,maxPt.y,maxVal);

    }
    
    // Curves
    normalizeScore(curvature);
    if(DEBUG_CURVE)
    {
        for(dx=-1; dx<=1; dx++)
            for(dy=-1; dy<=1; dy++)
            {
                printf("Curve normal now: (%d,%d) before: (%d,%d) after: (%d,%d): (%d,%d) %0.6f\n",now.x+dx,now.y+dy,before.x,before.y,after.x, after.y,dx+1,dy+1,curvature.at<float>(dx+1,dy+1));
            }
        myMinMaxLoc(curvature, minVal, maxVal, minPt, maxPt);
        printf("Curvature min at (%d,%d): %0.2f and max at (%d,%d): %0.2f\n",minPt.x,minPt.y,minVal,
               maxPt.x,maxPt.y,maxVal);
    }
    
    // Gradient
    gradientMagnitude.copyTo(imageScore);
    if(DEBUG_GRAD)
        for(dx=-1; dx<=1; dx++)
            for(dy=-1; dy<=1; dy++)
            {
                printf("Gradient magnitude at (%d,%d), (%d,%d): %0.6f\n",now.x+dx,now.y+dy,dx+1,dy+1,
                        imageScore.at<float>(dx+1,dy+1));
            }

    // Note: normalizeImageScore() takes negative of gradient magnitude so maximum gradient gives minimum energy
    normalizeImageScore(imageScore, CLAMP);
    if(DEBUG_GRAD)
    {
        for(dx=-1; dx<=1; dx++)
            for(dy=-1; dy<=1; dy++)
            {
                printf("Normalized gradient magnitude at (%d,%d), (%d,%d): %0.2f\n",now.x+dx,now.y+dy,dx+1,dy+1,
                       imageScore.at<float>(dx+1,dy+1));
            }
        myMinMaxLoc(curvature, minVal, maxVal, minPt, maxPt);
        printf("Gradient min at (%d,%d): %0.2f and max at (%d,%d): %0.6f\n",minPt.x,minPt.y,minVal,
               maxPt.x,maxPt.y,maxVal);
    }
    
    // Compute total energy
    totalScore = alpha * continuity + beta * curvature + gamma * imageScore;
    if(DEBUG)
        for(dx=-1; dx<=1; dx++)
            for(dy=-1; dy<=1; dy++)
            {
                printf("Scores at (%d,%d): %0.2f, %0.2f, %0.2f, %0.6f\n",now.x+dx,now.y+dy,
                   continuity.at<float>(dx+1,dy+1), curvature.at<float>(dx+1,dy+1),
                   imageScore.at<float>(dx+1,dy+1), totalScore.at<float>(dx+1,dy+1));
            }

    myMinMaxLoc(totalScore, minVal, maxVal, minPt, maxPt);
    out.x = now.x + minPt.x-1;
    out.y = now.y + minPt.y-1;
    
    if(DEBUG)
        printf("Orig point (%d,%d) and new point (%d,%d)\n",now.x,now.y,out.x,out.y);
}

//Given: management of contour evolution
double evolveContour(vector<Point>& contour, Mat& gray, double alpha, double beta, double gamma)
{
    if(DEBUG)printf("Inside evolveContour()\n");
    vector<Point> original = contour;
    Mat submatrix;
    
    int numPoints = (int)original.size()-1;
    if(numPoints <= 0)
    {
        return -1;
    }
    
    double meanDist = computeMeanPointDistance(original);
    Mat gradientMagnitude;
    gradientSobel(gray, gradientMagnitude);
    
    
    if(DEBUG)
        printf("Size of original: %ld numPoints: %d\n",original.size(),numPoints);
    for(int f=1; f<numPoints; f++)
    {
        if(DEBUG)printf("Original point: (%d,%d)\n",original[f].x, original[f].y);
        gradientMagnitude(Rect(original[f].x-1, original[f].y-1, 3, 3)).copyTo(submatrix);
        evolvePoint(gray, contour[f], original[f], original[f-1], original[f+1], submatrix, meanDist, alpha, beta, gamma);
    }
    gradientMagnitude(Rect(original[0].x-1, original[0].y-1, 3, 3)).copyTo(submatrix);
    evolvePoint(gray, contour[0], original[0], original[numPoints], original[1], submatrix, meanDist, alpha, beta, gamma);
    gradientMagnitude(Rect(original[numPoints].x-1, original[numPoints].y-1, 3, 3)).copyTo(submatrix);
    evolvePoint(gray, contour[numPoints], original[numPoints], original[numPoints-1], original[0], submatrix, meanDist, alpha, beta, gamma);
    
    double totalChange = 0;
    for(int f=0; f<contour.size(); f++)
    {
        totalChange += computeDistance(original[f], contour[f]);
    }
    return totalChange / double(contour.size());
}

//Helper function to compute the E_curv, curvature term given three points.
//The curvature is computed centered on the second point
double computeCurvature(Point p1, Point p2, Point p3)
{
    double x = p1.x - 2*p2.x + p3.x;
    double y = p1.y - 2*p2.y + p3.y;
    return x*x+y*y;
}

//Helper function to compute the distance between two points
double computeDistance(Point p1, Point p2)
{
    double x = p1.x - p2.x;
    double y = p1.y - p2.y;
    return sqrt(x*x+y*y);
}

//Helper function to compute the E_cont continuity term, which is a measure
//of the difference between the average distance between points on the contour an
//the distance between these two particular points
double computeContinuity(double meanDist, Point p1, Point p2)
{
    double dist = computeDistance(p1, p2);
    return abs(meanDist - dist);
}

//Helper function to normalize a matrix of scores stored in a Mat object
//by dividing by the largest value (for continuity and curvature terms)
void normalizeScore(Mat& score)
{
    double minVal, maxVal;
    Point minPt, maxPt;

    myMinMaxLoc(score, minVal, maxVal, minPt, maxPt);
    score = score/maxVal;
}

//Helper function to normalize a matrix of image scores stored in a Mat object
//by subtracting the minimum value and dividing by the difference between the
//max and min values
//The paper recommends a clamp value of 5, but I found that 30 worked better for
//the test images I have provided
void normalizeImageScore(Mat& score, double clamp)
{
    double minVal, maxVal;
    Point minPt, maxPt;

    myMinMaxLoc(score, minVal, maxVal, minPt, maxPt);
    score = (-score+minVal)/max(clamp, maxVal-minVal);
}

//Helper function to compute the average distance between points in the contour
double computeMeanPointDistance(vector<Point>& contour)
{
    int numPoints = (int)contour.size()-1;
    if(numPoints <= 0)
    {
        return -1;
    }
    
    double distAccumulator = 0;
    for(int f=0; f<numPoints; f++)
    {
        distAccumulator += computeDistance(contour[f], contour[f+1]);
    }
    distAccumulator += computeDistance(contour[0], contour[numPoints]);
    return distAccumulator / double(contour.size());
}



// Helper function to subsample the user-provided contour right after input
// (I found that my mouse events were really fast and every 10th point led to
//a reasonable-looking contour. Your mileage may vary
void subsampleContour(vector<Point>& contour, int subsample)
{
    vector<Point> original = contour;
    contour.clear();
    for(int i=0; i<original.size(); i+=2)
    {
        contour.push_back(original[i]);
    }
}

//Mouse handler to let the user draw the initial contour
//click to start and click again to stop
void onMouse( int event, int x, int y, int /*flags*/, void* /*param*/ )
{
    if( event == CV_EVENT_LBUTTONDOWN || event == CV_EVENT_RBUTTONDOWN)
    {
        bRecordContour = !bRecordContour;
        if(bRecordContour)
        {
            contour.clear();
        }
        else
        {
            subsampleContour(contour, 10);
        }
    }
    if( bRecordContour && event == CV_EVENT_MOUSEMOVE && x > 0 && y > 0)
    {
        contour.push_back(Point(x, y));
    }
}

void drawOutline(Mat& image, vector<Point>& outline)
{
    int numPoints = (int)outline.size()-1;
    if(numPoints <= 0)
    {
        return;
    }
    for(int f=0; f<numPoints; f++)
    {
        line(image, outline[f], outline[f+1], Scalar(255, 0, 0), 3);
        circle(image, outline[f+1], 3, Scalar(0, 255, 0), 1, 8);
    }
    line(image, outline[numPoints], outline[0], Scalar(255, 0, 0), 3);
    circle(image, outline[numPoints], 3, Scalar(0, 255, 0), 1, 8);
}

void gradientSobel(Mat& image,  Mat& magnitude)
{
    Mat dX, dY;
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

void fillContour()
{
    contour.push_back(Point(200, 200));
    contour.push_back(Point(220, 220));
    contour.push_back(Point(220, 230));
    contour.push_back(Point(200, 250));
    contour.push_back(Point(180, 225));
}
