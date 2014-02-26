#include "functions.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"
#include <iostream>


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
    for(int i = 0; i < grid_y; i++) {
        for(int j = 0; j < grid_x; j++) {
            Mat s = src[k++];
            resize(s,s,Size(width,height));
            s.copyTo(dst(Rect(j*width,i*height,width,height)));
        }
    }
}
