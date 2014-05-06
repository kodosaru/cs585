#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

using namespace cv;
using namespace std;

void printMat(Mat& A);
void printMatTruncSmall(Mat& A);
void cameraProjectPoints(Mat& P, Mat& x, Mat& u);
void fundamentalMatrix(Mat& P0, Mat& P1, Mat& t0, Mat& F);
void reconstruct3DPoint(Mat& P0, Mat& P1, Mat& u0, Mat& u1, Mat& X);

int main(int argc, char* argv[])
{
    // You will need to do some work to compute the camera matrices from the numbers given in the homework.

    // modified by donj (all below)
    // Internal calibration matrix
    double data1[9] =  {1250,         0,         500,
                        0,        1250,         500,
                        0,           0,           1};
    Mat K = Mat(3,3, CV_64F, data1);
    cout<<endl<<"The internal calibration matrix K: "<<endl;
    printMat(K);
    
    // Augmented identity matrix
    double data2[12]= {1,     0,     0,     0,
                        0,     1,     0,     0,
                        0,     0,     1,     0};
    Mat I_Augmented=Mat(3,4, CV_64F, data2);
    cout<<endl<<"The augmented identity matrix I3_Aug used to create P0: "<<endl;
    printMat(I_Augmented);
 
    // P1 rotation matrix
    double data3[9] = {0.8750000000, 0.0000000000, -0.4841229183,
                       0.0000000000, 1.0000000000, 0.0000000000,
                       0.4841229183, 0.0000000000, 0.8750000000};
    Mat Rp1 = Mat(3,3, CV_64F, data3);
    cout<<endl<<"The rotation matrix Rp1 used to create P1: "<<endl;
    printMat(Rp1);

    // P1 translation matrix
    double data4[3] = {4.8412291828,	0.0000000000,	1.2500000000};
    Mat Tp1 = Mat(3,1, CV_64F, data4);
    cout<<endl<<"The translation matrix Tp1 used to create P1: "<<endl;
    printMat(Tp1);

    // PO camera matrix
    Mat P0 = K * I_Augmented;
    cout<<endl<<"The P0 camera matrix = K * I3_Aug: "<<endl;
    printMat(P0);
    
    // P1 camera matrix
    Mat CPp1;
    hconcat(Rp1, Tp1, CPp1);
    Mat P1 = K * CPp1;
    cout<<endl<<"The P1 camera matrix = K * [Rp1 | Tp1]: "<<endl;
    printMat(P1);

    double pt0[4] = {0, 0, 10, 1};
    Mat X0(4,1, CV_64F, pt0);
    double pt1[4] = {0, 3, 10, 1};
    Mat X1(4,1, CV_64F, pt1);
    double pt2[4] = {0, 2.4253, 8.0843, 1};
    Mat X2(4,1, CV_64F, pt2);
    double pt3[4] = {0, 3.5747, 11.9157};
    Mat X3(4,1, CV_64F, pt3);
    double pt4[4] = {0.9274, 2.4253, 8.3238};
    Mat X4(4,1, CV_64F, pt4);
    double pt5[4] = {-0.9274, 3.5747, 11.6762};
    Mat X5(4,1, CV_64F, pt5);

    Mat u0_0, u0_1, u1_0, u1_1, u2_0, u2_1, u3_0, u3_1, u4_0, u4_1, u5_0, u5_1;
    cameraProjectPoints(P0, X0, u0_0);
    cameraProjectPoints(P1, X0, u0_1);
    cameraProjectPoints(P0, X1, u1_0);
    cameraProjectPoints(P1, X1, u1_1);
    cameraProjectPoints(P0, X2, u2_0);
    cameraProjectPoints(P1, X2, u2_1);
    cameraProjectPoints(P0, X3, u3_0);
    cameraProjectPoints(P1, X3, u3_1);
    cameraProjectPoints(P0, X4, u4_0);
    cameraProjectPoints(P1, X4, u4_1);
    cameraProjectPoints(P0, X5, u5_0);
    cameraProjectPoints(P1, X5, u5_1);

    cout<<endl;
    cout<<"Image plane point u0_0 corresponding to the fixation point x0"<<endl;
    printMat(u0_0);
    cout<<"Image plane point u0_1 corresponding to the fixation point x0"<<endl;
    printMat(u0_1);
    cout<<endl;
    cout<<"Image plane point u1_0 corresponding to world point x1"<<endl;
    printMat(u1_0);
    cout<<"Image plane point u1_1 corresponding to world point x1"<<endl;
    printMat(u1_1);
    cout<<endl;
    cout<<"Image plane point u2_0 corresponding to world point x2"<<endl;
    printMat(u2_0);
    cout<<"Image plane point u2_1 corresponding to world point x2"<<endl;
    printMat(u2_1);
    cout<<endl;
    cout<<"Image plane point u3_0 corresponding to world point x3"<<endl;
    printMat(u3_0);
    cout<<"Image plane point u3_1 corresponding to world point x3"<<endl;
    printMat(u3_1);
    cout<<endl;
    cout<<"Image plane point u4_0 corresponding to world point x4"<<endl;
    printMat(u4_0);
    cout<<"Image plane point u4_1 corresponding to world point x4"<<endl;
    printMat(u4_1);
    cout<<endl;
    cout<<"Image plane point u5_0 corresponding to world point x5"<<endl;
    printMat(u5_0);
    cout<<"Image plane point u5_1 corresponding to world point x5"<<endl;
    printMat(u5_1);

    cout<<endl;
    Mat F;
    Mat t0 = Mat::zeros(3,1,CV_64F);
    fundamentalMatrix(P0, P1, t0, F);
    cout<<"The fundamental matrix computed from P0, P1, and t0:"<<endl;
    printMat(F);
    cout<<endl;

    
    Mat X0a, X1a, X2a, X3a, X4a, X5a;
    reconstruct3DPoint(P0, P1, u0_0, u0_1, X0a);
    cout<<"X0 derived from the camera matrices and u0_0 and u0_1"<<endl;
    printMatTruncSmall(X0a);
    cout<<endl;
    reconstruct3DPoint(P0, P1, u1_0, u1_1, X1a);
    cout<<"X1 derived from the camera matrices and u1_0 and u1_1"<<endl;
    printMatTruncSmall(X1a);
    cout<<endl;
    reconstruct3DPoint(P0, P1, u2_0, u2_1, X2a);
    cout<<"X2 derived from the camera matrices and u2_0 and u2_1"<<endl;
    printMatTruncSmall(X2a);
    cout<<endl;
    reconstruct3DPoint(P0, P1, u3_0, u3_1, X3a);
    cout<<"X3 derived from the camera matrices and u3_0 and u3_1"<<endl;
    printMatTruncSmall(X3a);
    cout<<endl;
    reconstruct3DPoint(P0, P1, u4_0, u4_1, X4a);
    cout<<"X4 derived from the camera matrices and u4_0 and u4_1"<<endl;
    printMatTruncSmall(X4a);
    cout<<endl;
    reconstruct3DPoint(P0, P1, u5_0, u5_1, X5a);
    cout<<"X5 derived from the camera matrices and u5_0 and u5_1"<<endl;
    printMatTruncSmall(X5a);
    cout<<endl;
}

void printMat(Mat& A)
{
    for(int r=0; r<A.rows; r++)
    {
        for(int c=0; c<A.cols; c++)
        {
            cout<<A.at<double>(r,c)<<'\t';
        }
        cout<<endl;
    }
}

void printMatTruncSmall(Mat& A)
{
    for(int r=0; r<A.rows; r++)
    {
        for(int c=0; c<A.cols; c++)
        {
            if(A.at<double>(r,c)<1e-10 && A.at<double>(r,c)>-1e-10)
                cout<<0.0<<'\t';
            else
                cout<<A.at<double>(r,c)<<'\t';
        }
        cout<<endl;
    }
}

void cameraProjectPoints(Mat& P, Mat& x, Mat& u)
{
    Mat X(4,1, CV_64F);
    for(int i=0; i<3; i++)
    {
        X.at<double>(i) = x.at<double>(i);
    }
    X.at<double>(3) = 1;

    Mat U = P*X;
    if(u.empty())
    {
        u.create(2,1, CV_64F);
    }
    u.at<double>(0) = U.at<double>(0)/U.at<double>(2);
    u.at<double>(1) = U.at<double>(1)/U.at<double>(2);
 }

void fundamentalMatrix(Mat& P0, Mat& P1, Mat& t0, Mat& F)
{
    Mat e1;
    cameraProjectPoints(P1, t0, e1);

    Mat ex=Mat::zeros(3,3,CV_64F);
    ex.at<double>(0,1) = -1;
    ex.at<double>(0,2) = e1.at<double>(1);
    ex.at<double>(1,0) = 1;
    ex.at<double>(1,2) = -e1.at<double>(0);
    ex.at<double>(2,0) = -e1.at<double>(1);
    ex.at<double>(2,1) = e1.at<double>(0);

    Mat P0_P0T = P0 * P0.t();
    Mat P0Plus = P0.t()*P0_P0T.inv();

    F = ex*P1*P0Plus;
}

void reconstruct3DPoint(Mat& P0, Mat& P1, Mat& u0, Mat& u1, Mat& X)
{
    Mat A(4,4,CV_64F);
    A.row(0) = u0.at<double>(0)*P0.row(2)-P0.row(0);
    A.row(1) = u0.at<double>(1)*P0.row(2)-P0.row(1);
    A.row(2) = u1.at<double>(0)*P1.row(2)-P1.row(0);
    A.row(3) = u1.at<double>(1)*P1.row(2)-P1.row(1);
    SVD SVDA(A);
    Mat h = SVDA.vt.row(3);
    if(X.empty())
    {
        X.create(3,1, CV_64F);
    }
    X.at<double>(0)=h.at<double>(0)/h.at<double>(3);
    X.at<double>(1)=h.at<double>(1)/h.at<double>(3);
    X.at<double>(2)=h.at<double>(2)/h.at<double>(3);
}