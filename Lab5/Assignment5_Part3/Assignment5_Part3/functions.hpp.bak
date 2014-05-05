#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "opencv2/core/core.hpp"

using namespace cv;
using namespace std;

Mat getScaleMatrix(double scaleFactor);
Mat getTranslationMatrix(Point2f offset);
Mat myGetRotationMatrix2D(Point2f center, double rotationAngle, double scaleFactor);
Mat getShearMatrix2D(Point2f center, double XShearFactor, double YShearFactor, double scaleFactor);
void printTransformMat(Mat mat);
unsigned long listFiles(String targetPath, vector<string>& fileNames);
void tile(const vector<Mat> &src, Mat &dst, int grid_x, int grid_y);
void resizeImageRelativeToBackground(Point sizeBackground, float factor, Mat& image);
void mosaic(const vector<Mat> &src, Mat &dst, int count);
Mat createMask(Mat& image);
#endif
