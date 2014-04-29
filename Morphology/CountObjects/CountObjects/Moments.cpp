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

// Orientaton Angle
Mat* orientation(vector<PIXEL> v)
{
    Mat *m=new Mat(2,1,CV_64FC1);
    double mup20 = muPrimeij(v, 2, 0);
    double mup02 = muPrimeij(v, 0, 2);
    if(mup20 == mup02)
    {
        cout<<"Orientation cannot be computed because muPrime20 equals muPrime02"<<endl;
        m->at<double>(0,0) = -DBL_MAX;
        m->at<double>(1,0) = -DBL_MAX;
        return m;
    }
    // range -90 to 90 deg
    m->at<double>(0,0) = 0.5 * atan( (2.0 * muPrimeij(v, 1, 1)) / (mup20 - mup02) );
    m->at<double>(1,0) = 180.0/M_PI * m->at<double>(0,0);
    return m;
}

// Eccentricity
double eccentricity(vector<PIXEL> v)
{
    Mat m = *eigenvalueMatrix(v);
    double lambda2 = m.at<double>(0,0);
    double lambda1 = m.at<double>(1,0);
    
    return sqrt(1.0 - lambda2/lambda1);
}

// Eigenvalue Matrix
Mat* eigenvalueMatrix(vector<PIXEL> v)
{
    Mat *m=new Mat(2,1,CV_64FC1);
    double leftTerm = muPrimeij(v, 2, 0) + muPrimeij(v, 0, 2);
    leftTerm /= 2.0;
    double rightTerm = sqrt(4.0 * POW2(muPrimeij(v, 1, 1)) + POW2(muPrimeij(v, 2, 0) + muPrimeij(v, 0, 2)));
    rightTerm /= 2.0;
    m->at<double>(0,0) = leftTerm - rightTerm;
    m->at<double>(1,0) = leftTerm + rightTerm;

    return m;
}

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
        return v.size();
    }
    //M01
    else if(i==0 && j==1)
    {
        for(vector<PIXEL>::iterator it = v.begin(); it != v.end(); ++it) {
            sum += POW1((*it).pt.y);
        }
        
    }
    //M10
    else if(i==1 && j==0)
    {
        for(vector<PIXEL>::iterator it = v.begin(); it != v.end(); ++it) {
            sum += POW1((*it).pt.x);
        }
        
    }
    //M11
    else if(i==1 && j==1)
    {
        for(vector<PIXEL>::iterator it = v.begin(); it != v.end(); ++it) {
            sum += POW1((*it).pt.x) * POW1((*it).pt.y);
        }
        
    }
    //M20
    else if(i==2 && j==0)
    {
        for(vector<PIXEL>::iterator it = v.begin(); it != v.end(); ++it) {
            sum += POW2((*it).pt.x);
        }
        
    }
    //M02
    else if(i==0 && j==2)
    {
        for(vector<PIXEL>::iterator it = v.begin(); it != v.end(); ++it) {
            sum += POW2((*it).pt.y);
        }
        
    }
    //M21
    else if(i==2 && j==1)
    {
        for(vector<PIXEL>::iterator it = v.begin(); it != v.end(); ++it) {
            sum += POW2((*it).pt.x) * POW1((*it).pt.y);
        }
        
    }
    //M12
    else if(i==1 && j==2)
    {
        for(vector<PIXEL>::iterator it = v.begin(); it != v.end(); ++it) {
            sum += POW1((*it).pt.x) * POW2((*it).pt.y);
        }
        
    }
    //M30
    else if(i==3 && j==0)
    {
        for(vector<PIXEL>::iterator it = v.begin(); it != v.end(); ++it) {
            sum += POW3((*it).pt.x);
        }
        
    }
    //M03
    else if(i==0 && j==3)
    {
        for(vector<PIXEL>::iterator it = v.begin(); it != v.end(); ++it) {
            sum += POW3((*it).pt.y);
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
        return Mij(v, 3, 0) - 3.0 * xbar(v) * Mij(v, 2, 0) +  2.0 * POW2(xbar(v)) * Mij(v, 1, 0);
    }
    //mu03
    else if(i==0 && j==3)
    {
        return Mij(v, 0, 3) - 3.0 * ybar(v) * Mij(v, 0, 2) +  2.0 * POW2(ybar(v)) * Mij(v, 0, 1);
    }
    else
    {
        cout<<"Invalid central moment("<<i<<","<<j<<")"<<endl;
        return -DBL_MAX;
    }
}

// Hu Translation, scale, and rotation invarient moments (+I8 recommended by Flusser & Suk)
double Hui(vector<PIXEL> v, long i)
{
    double n02 = etaij(v, 0, 2);
    double n03 = etaij(v, 0, 3);
    double n11 = etaij(v, 1, 1);
    double n12 = etaij(v, 1, 2);
    double n20 = etaij(v, 2, 0);
    double n21 = etaij(v, 2, 1);
    double n30 = etaij(v, 3, 0);
    
    double ap = n20 + n02;
    double am = n20 - n02;
    double bp = n30 + n12;
    double cp = n21 + n03;
    double dm = n30 - 3.0 * n12;
    double em = 3.0 * n21 - n03;
    double fp = n03 + n21;
 
    switch (i) {
        case 1:
            return ap;
            break;
            
        case 2:
            return POW2(am) + 4.0 * POW2(n11);
            break;
            
        case 3:
            return POW2(dm) + POW2(em);
            break;
            
        case 4:
            return POW2(bp) + POW2(cp);
            break;
            
        case 5:
            return dm * bp * ( POW2(bp) - 3 * POW2(cp) ) + em * cp * ( 3.0 * POW2(bp) - POW2(cp) );
            break;

        case 6:
            return am * ( POW2(bp) - POW2(cp) ) + 4 * n11 * bp * cp;
            break;

        case 7:
            return em * bp * ( POW2(bp) - 3.0 * POW2(cp) ) - dm * cp * ( 3.0 * POW2(bp) - POW2(cp) );
            break;

        case 8:
            return n11 * ( POW2(bp) + POW2(fp) ) - ap * bp * fp;
            break;
            
        default:
            cout<<"Invalid Hu moment "<<i<<endl;
            return -DBL_MAX;
            break;
    }

}

// Raw moments which include pixel intensity in calculations
double MijIxy(vector<PIXEL> v, long i, long j)
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

