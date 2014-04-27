//
//  Moments.cpp
//  CountObjects
//
//  Created by Don Johnson on 4/26/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//
//  From http://en.wikipedia.org/wiki/Image_moment

#include "Moments.h"
#include "FloodFillMethods.h"
#include "math.h"

using namespace cv;
using namespace std;

#define POW1(val) (val)
#define POW2(val) (val * val)
#define POW3(val) (val * val * val)

// Covariance Matrix
Mat* covarianceMatrix(vector<PIXEL> v)
{
    Mat *m=new Mat(2,2,CV_64FC1);
    m->at<double>(0,0) = muPrimeij(v, 2, 0);
    m->at<double>(0,1) = muPrimeij(v, 1, 1);
    m->at<double>(1,0) = muPrimeij(v, 1, 1);
    m->at<double>(1,1) = muPrimeij(v, 0, 2);
    return m;
}

// Central moments divided by sum of the pixel values M00
double muPrimeij(vector<PIXEL> v, long i, long j)
{
    return muij(v, i, j) / muij(v, 0, 0);
}

// Scale invariant moments
double etaij(vector<PIXEL> v, long i, long j)
{
    return muij(v, i, j) / pow(muij(v, 0, 0), 1 + (i + j)/2.0);
}

// Raw moments
double Mij(vector<PIXEL> v, long i, long j)
{
    double sum=0.0;
    
    //M00
    if(i==0 && j==0)
    {
        for(vector<PIXEL>::iterator it = v.begin(); it != v.end(); ++it) {
            sum += (*it).val[0];
        }
        
    }
    //M01
    else if(i==0 && j==1)
    {
        for(vector<PIXEL>::iterator it = v.begin(); it != v.end(); ++it) {
            sum += POW1((*it).pt.y) * (*it).val[0];
        }
        
    }
    //M10
    else if(i==1 && j==0)
    {
        for(vector<PIXEL>::iterator it = v.begin(); it != v.end(); ++it) {
            sum += POW1((*it).pt.x) * (*it).val[0];
        }
        
    }
    //M11
    else if(i==1 && j==1)
    {
        for(vector<PIXEL>::iterator it = v.begin(); it != v.end(); ++it) {
            sum += POW1((*it).pt.x) * POW1((*it).pt.y) * (*it).val[0];
        }
        
    }
    //M20
    else if(i==2 && j==0)
    {
        for(vector<PIXEL>::iterator it = v.begin(); it != v.end(); ++it) {
            sum += POW2((*it).pt.x) * (*it).val[0];
        }
        
    }
    //M02
    else if(i==0 && j==2)
    {
        for(vector<PIXEL>::iterator it = v.begin(); it != v.end(); ++it) {
            sum += POW2((*it).pt.y) * (*it).val[0];
        }
        
    }
    //M21
    else if(i==2 && j==1)
    {
        for(vector<PIXEL>::iterator it = v.begin(); it != v.end(); ++it) {
            sum += POW2((*it).pt.x) * POW1((*it).pt.y) * (*it).val[0];
        }
        
    }
    //M12
    else if(i==1 && j==2)
    {
        for(vector<PIXEL>::iterator it = v.begin(); it != v.end(); ++it) {
            sum += POW1((*it).pt.x) * POW2((*it).pt.y) * (*it).val[0];
        }
        
    }
    //M30
    else if(i==3 && j==0)
    {
        for(vector<PIXEL>::iterator it = v.begin(); it != v.end(); ++it) {
            sum += POW3((*it).pt.x) * (*it).val[0];
        }
        
    }
    //M03
    else if(i==0 && j==3)
    {
        for(vector<PIXEL>::iterator it = v.begin(); it != v.end(); ++it) {
            sum += POW3((*it).pt.y) * (*it).val[0];
        }
        
    }
    else
    {
        cout<<"Invalid raw moment("<<i<<","<<j<<")"<<endl;
        return -DBL_MAX;
    }
    return sum;
}

double xbar(vector<PIXEL> v)
{
    return Mij(v, 1, 0)/Mij(v, 0, 0);
}

double ybar(vector<PIXEL> v)
{
    return Mij(v, 0, 1)/Mij(v, 0, 0);
}

// Central moments
double muij(vector<PIXEL> v, long i, long j)
{
    //mu00
    if(i==0 && j==0)
    {
        return Mij(v, 0, 0);
        
    }
    //mu01
    else if(i==0 && j==1)
    {
        return 0.0;
        
    }
    //mu10
    else if(i==1 && j==0)
    {
        return 0.0;
    }
    //mu11
    else if(i==1 && j==1)
    {
        return (Mij(v, 1, 1) - xbar(v) * Mij(v, 0, 1));

    }
    //mu20
    else if(i==2 && j==0)
    {
        return Mij(v, 2, 0) - xbar(v) * Mij(v, 1, 0);
    }
    //mu02
    else if(i==0 && j==2)
    {
        return Mij(v, 0, 2) - ybar(v) * Mij(v, 0, 1);
    }
    //mu21
    else if(i==2 && j==1)
    {
        return Mij(v, 2, 1) - 2.0 * xbar(v) * Mij(v, 1, 1) -  ybar(v) * Mij(v, 2, 0) + 2.0 * POW2(xbar(v)) * Mij(v, 0, 1);
    }
    //mu12
    else if(i==1 && j==2)
    {
        return Mij(v, 1, 2) - 2.0 * ybar(v) * Mij(v, 1, 1) -  xbar(v) * Mij(v, 0, 2) + 2.0 * POW2(ybar(v)) * Mij(v, 1, 0);
    }
    //mu30
    else if(i==3 && j==0)
    {
        return Mij(v, 3, 0) - 3.0 * xbar(v) * Mij(v, 2, 0) -  2.0 * POW2(xbar(v)) * Mij(v, 1, 0);
    }
    //mu03
    else if(i==0 && j==3)
    {
        return Mij(v, 0, 3) - 3.0 * ybar(v) * Mij(v, 0, 2) -  2.0 * POW2(ybar(v)) * Mij(v, 0, 1);
    }
    else
    {
        cout<<"Invalid central moment("<<i<<","<<j<<")"<<endl;
        return -DBL_MAX;
    }
}

