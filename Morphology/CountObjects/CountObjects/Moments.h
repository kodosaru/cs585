//
//  Moments.h
//  CountObjects
//
//  Created by Don Johnson on 4/26/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#ifndef __CountObjects__Moments__
#define __CountObjects__Moments__

#include <iostream>
#include "FloodFillMethods.h"

#define POW1(val) ((val))
#define POW2(val) ((val) * (val))
#define POW3(val) ((val) * (val) * (val))

// Hu Translation, scale, and rotation invarient moments (+I8 recommended by Flusser & Suk)
double Hui(cv::vector<PIXEL> v, long i);
// Orientaton Angle
cv::Mat* orientation(cv::vector<PIXEL> v);
// Eccentricity
double eccentricity(cv::vector<PIXEL> v);
// Eigenvalue Matrix
cv::Mat* eigenvalueMatrix(cv::vector<PIXEL> v);
// Covariance Matrix
cv::Mat* covarianceMatrix(cv::vector<PIXEL> v);
// Central moments divided by sum of the pixel values M00
double muPrimeij(cv::vector<PIXEL> v, long i, long j);
// Scale invariant moments
double etaij(cv::vector<PIXEL> v, long i, long j);
// Raw moments
double Mij(cv::vector<PIXEL> v, long i, long j);
// Raw moments which include pixel intensity in calculations
double MijIij(cv::vector<PIXEL> v, long i, long j);
// Centroid coordinates
double xbar(cv::vector<PIXEL> v);
double ybar(cv::vector<PIXEL> v);
// Central moments
double muij(cv::vector<PIXEL> v, long i, long j);
#endif /* defined(__CountObjects__Moments__) */
