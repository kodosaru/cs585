#include "functions.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;
namespace fs = boost::filesystem;

Mat getTranslationMatrix(Point2f offset)
{
    Mat translation  = Mat::eye(Size(3,3), CV_64FC1);
    translation.at<double>(0, 2) = offset.x;
    translation.at<double>(1, 2) = offset.y;
    return translation;
}

Mat getScaleMatrix(double scaleFactor)
{
    Mat scale  = Mat::eye(Size(3,3), CV_64FC1);
    scale.at<double>(0, 0) = scaleFactor;
    scale.at<double>(0, 1) = 0.0;
    scale.at<double>(1, 0) = 0.0;
    scale.at<double>(1, 1) = scaleFactor;
    return scale;
}

Mat myGetRotationMatrix2D(Point2f center, double rotationAngle, double scaleFactor)
{
    Mat mat1 = Mat::eye(Size(3,3), CV_64FC1);
    Mat mat2 = Mat::eye(Size(3,3), CV_64FC1);
    Mat mat3 = Mat::eye(Size(3,3), CV_64FC1);
    Mat mat4 = Mat::eye(Size(3,3), CV_64FC1);
    
    // Move image to origin
    mat1.at<double>(0, 2) = -center.x;
    mat1.at<double>(1, 2) = -center.y;

    // Rotate image
    double theta = rotationAngle * M_PI / 180.0;
    mat2.at<double>(0, 0) = cos(theta);
    mat2.at<double>(0, 1) = sin(theta);
    mat2.at<double>(1, 0) = -sin(theta);
    mat2.at<double>(1, 1) = cos(theta);
    
    // Scale image
    mat3.at<double>(0, 0) = scaleFactor;
    mat3.at<double>(1, 1) = scaleFactor;
    
    // Move back to original location
    mat4.at<double>(0, 2) = center.x;
    mat4.at<double>(1, 2) = center.y;
    
    // Return composition of matrices
    return mat4 * (mat3 * (mat2 * mat1));
}

Mat getShearMatrix2D(Point2f center, double XShearFactor, double YShearFactor, double scaleFactor)
{
    Mat mat1 = Mat::eye(Size(3,3), CV_64FC1);
    Mat mat2 = Mat::eye(Size(3,3), CV_64FC1);
    Mat mat3 = Mat::eye(Size(3,3), CV_64FC1);
    Mat mat4 = Mat::eye(Size(3,3), CV_64FC1);
    
    // Move image to origin
    mat1.at<double>(0, 2) = -center.x;
    mat1.at<double>(1, 2) = -center.y;
    
    // Shear image
    mat2.at<double>(0, 1) = XShearFactor;
    mat2.at<double>(1, 0) = YShearFactor;
    
    // Scale image
    mat3.at<double>(0, 0) = scaleFactor;
    mat3.at<double>(1, 1) = scaleFactor;
    
    // Move back to original location
    mat4.at<double>(0, 2) = center.x;
    mat4.at<double>(1, 2) = center.y;
    
    // Return composition of matrices
    return mat4 * (mat3 * (mat2 * mat1));
}

void printTransformMat(Mat mat)
{
    cout << "Transform matrix:" << endl;
    cout << mat.at<double>(0,0) << "\t" <<  mat.at<double>(0,1) << "\t" <<  mat.at<double>(0,2) << endl;
    cout << mat.at<double>(1,0) << "\t" <<  mat.at<double>(1,1) << "\t" <<  mat.at<double>(1,2) << endl;
}

void resizeImageRelativeToBackground(Point sizeBackground, float factor, Mat& image)
{
    float imageAspect = image.cols/(float)image.rows;
    float ratioWidth = image.cols/(float)sizeBackground.x;
    float ratioHeight = image.rows/(float)sizeBackground.y;
    float resizeFactor, newWidth, newHeight;

    if(ratioWidth>ratioHeight)
    {
        resizeFactor = sizeBackground.x/(image.cols * factor);
        newWidth = resizeFactor * image.cols;
        newHeight = newWidth / imageAspect;
    }
    else
    {
        resizeFactor = sizeBackground.y/(image.rows * factor);
        newHeight = resizeFactor * image.rows;
        newWidth = newHeight * imageAspect;
    }
    resize(image, image, Size(newWidth,newHeight));
}

unsigned long listFiles(String targetPath, vector<string>& fileNames)
{
    boost::progress_timer t( std::clog );
    
    fs::path full_path( fs::initial_path<fs::path>() );
    full_path = fs::system_complete( fs::path( targetPath ) );
    
    unsigned long file_count = 0;
    unsigned long dir_count = 0;
    unsigned long other_count = 0;
    unsigned long err_count = 0;
    
    if ( !fs::exists( full_path ) )
    {
        cout << "\nNot found: " << full_path.string() << std::endl;
        return 1;
    }
    
    if ( fs::is_directory( full_path ) )
    {
        cout << "\nIn directory: "
        << full_path.string() << "\n\n";
        fs::directory_iterator end_iter;
        for ( fs::directory_iterator dir_itr( full_path );
             dir_itr != end_iter;
             ++dir_itr )
        {
            try
            {
                if ( fs::is_directory( dir_itr->status() ) )
                {
                    ++dir_count;
                    std::cout << dir_itr->path().filename() << " [directory]\n";
                }
                else if ( fs::is_regular_file( dir_itr->status() ) )
                {
                    ++file_count;
                    std::cout << dir_itr->path().filename() << "\n";
                    fileNames.push_back(dir_itr->path().filename().string());
                }
                else
                {
                    ++other_count;
                    std::cout << dir_itr->path().filename() << " [other]\n";
                }
                
            }
            catch ( const std::exception & ex )
            {
                ++err_count;
                std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
            }
        }
        std::cout << "\n" << file_count << " files\n"
        << dir_count << " directories\n"
        << other_count << " others\n"
        << err_count << " errors\n";
    }
    else // must be a file
    {
        ++file_count;
        std::cout << "\nFound: " << full_path.string() << "\n";
        fileNames.push_back(full_path.string());
    }
    return file_count;
}

void tile(const vector<Mat> &src, Mat &dst, int grid_x, int grid_y) {
    // patch size
    int width  = dst.cols/grid_x;
    int height = dst.rows/grid_y;
    // iterate through grid
    int k = 0;
    char stemp[100];
    for(int i = 0; i < grid_y; i++) {
        for(int j = 0; j < grid_x; j++) {
            Mat s = src[k];
            resize(s,s,Size(width,height));
            Rect dstRect=Rect(j*width,i*height,width,height);
            sprintf(stemp, "Dst rect coordinates (%d,%d,%d,%d)",dstRect.x,dstRect.y,dstRect.width,dstRect.height);
            cout << stemp << endl;
            s.copyTo(dst(dstRect));
            k++;
        }
    }
}

void mosaic(const vector<Mat> &src, Mat &dst, int count) {
    int grid_y = (int)(sqrt(count));
    int grid_x = grid_y;
    int width  = dst.cols/grid_x;
    int height = dst.rows/grid_y;
    Mat temp,temp2,centerImage, centerImageMask;
    Rect centerImageRect;
    
    int k = 0;
    int xOverlap = 150;
    int yOverlap = 150;
    int xOffset = 100;
    int yOffset = 100;
    char stemp[100];
    vector<Mat> images;
    for(int i = 0; i < grid_y; i++) {
        for(int j = 0; j < grid_x; j++) {
            Mat s = src[k];
            cout<<"Before size ("<< s.cols<<","<<s.rows<<")"<<endl;
            double resizeFactor = 2.5;
            
            if(k==0) resizeFactor = 2.0;
            if(k==6) resizeFactor = 1.7;
            resizeImageRelativeToBackground(Point(dst.cols,dst.rows), resizeFactor, s);
            cout<<"After size ("<< s.cols<<","<<s.rows<<")"<<endl;
            
            int diag = sqrt(pow(s.cols,2)+pow(s.rows,2))+0.5;
            int minDim = min(s.rows,s.cols);
            double scaleFactor = minDim/(double)diag;
            Mat temp(s.size(),CV_8UC3);
            temp=Scalar(0);
            
            Rect dstRect=Rect(xOffset+j*width-j*xOverlap,yOffset+i*height-i*yOverlap,temp.cols, temp.rows);
            sprintf(stemp, "Dst rect coordinates (%d,%d,%d,%d)",dstRect.x,dstRect.y,dstRect.width,dstRect.height);
            cout << stemp << endl;
            
            Mat transform;
            switch (k)
            {
                case 0:
                case 2:
                case 6:
                case 8:
                {
                    double theta = k==2||k==6 ? -15.0 : 45.0;
                    transform = myGetRotationMatrix2D(Point2f(s.cols/2, s.rows/2), theta, scaleFactor);
                    warpPerspective(s, temp, transform, temp.size(), INTER_CUBIC, BORDER_TRANSPARENT);
                    break;
                }
                case 4:
                {
                    transform = getShearMatrix2D(Point(s.cols/2, s.rows/2),-0.2, -0.2, 0.8);
                    warpPerspective(s, temp, transform, temp.size(), INTER_CUBIC, BORDER_TRANSPARENT);
                    break;
                }
                case 1:
                case 3:
                case 5:
                case 7:
                case 9:
                {
                    s.copyTo(temp);
                    break;
                }
            }
            
            // Create mask
            Mat mask(temp.size(),CV_8UC1);
            cvtColor(temp,mask,CV_RGB2GRAY);
            threshold(mask,mask,0,255,THRESH_BINARY);

            if(k==0 || k==6)
            {
                Rect roi(200,0,temp.cols-200,temp.rows);
                temp(roi).copyTo(temp2);
                Rect newDst=Rect(dstRect.x,dstRect.y,dstRect.width-200,dstRect.height);
                temp2.copyTo(dst(newDst),createMask(temp2));
            }
            else if (k==4)
            {
                centerImage.create(temp.size(),CV_8UC3);
                temp.copyTo(centerImage);
                centerImageMask.create(mask.size(),CV_8UC1);
                mask.copyTo(centerImageMask);
                centerImageRect = dstRect;
            }
            else
            {
                temp.copyTo(dst(dstRect),createMask(temp));
            }
            temp.release();
            mask.release();
            k++;
        }
    }
    centerImage.copyTo(dst(centerImageRect),centerImageMask);
}

Mat createMask(Mat& image)
{
    Mat mask(image.size(),CV_8UC1);
    cvtColor(image,mask,CV_RGB2GRAY);
    threshold(mask,mask,0,255,THRESH_BINARY);
    return mask;
}


