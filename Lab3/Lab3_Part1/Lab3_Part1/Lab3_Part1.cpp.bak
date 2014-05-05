// Lab3_Part1.cpp : Defines the entry point for the console application.
//


#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <string>
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;

void makeIntVector(vector<int>& integers);
void constructOutline(vector<Point>& outline);
void drawOutline(Mat& image, vector<Point>& outline);

int main(int argc, char* argv[])
{

    //STL vectors in C++ are sortof like arrays that know their size.
    //They are templated on type, and dynamically resize themselves
    //when you add things to them.

    vector<int> numbers;
    makeIntVector(numbers);
    for(int i=0; i<numbers.size(); i++)
    {
        cout<<numbers[i]<<" ";
    }
    cout<<endl;


    Mat image(200, 200, CV_8UC3, Scalar(0));

    vector<Point> outline;
    constructOutline(outline);
    drawOutline(image, outline);

    imshow("ImageView", image);
    waitKey(0);

	return 0;
}

void makeIntVector(vector<int>& integers)
{
    integers.push_back(1);
    integers.push_back(2);
    integers.push_back(3);
    integers.push_back(4);
    integers.push_back(5);
}

void constructOutline(vector<Point>& square)
{
    square.push_back(Point(25, 25));
    square.push_back(Point(25, 75));
    square.push_back(Point(75, 75));
    square.push_back(Point(75, 25));
    square.push_back(Point(25, 25));
}

void drawOutline(Mat& image, vector<Point>& outline)
{
    for(int f=0; f<(outline.size()-1); f++)
    {
        line(image, outline[f], outline[f+1], Scalar(255, 0, 0), 3);
    }
}


