#define AFFINE_TRANSFORM_HPP
#ifndef AFFINE_TRANSFORM_HPP
void printTransformMat(Mat mat)
Mat getScaleMatrix(double scaleFactor);
Mat getTranslationMatrix(Point2f offset);
Mat getRotationMatrix2D(Point2f center, double rotationAngle, double scaleFactor)
Mat getShearMatrix2D(Point2f center, double XShearFactor, double YShearFactor, double scaleFactor);
#endif
