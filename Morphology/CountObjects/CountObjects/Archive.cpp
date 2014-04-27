//
//  Archive.cpp
//  KMeans
//
//  Created by Don Johnson on 4/21/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#include "Archive.hpp"
#include "opencv2/core/core.hpp"
#include <string>
#include <fstream>

void saveMat(cv::Mat& m, std::string filename) {
    std::ofstream ofs(filename.c_str());
    boost::archive::binary_oarchive oa(ofs);
    //boost::archive::text_oarchive oa(ofs);
    oa << m;
}

void loadMat(cv::Mat& m, std::string filename) {
    std::ifstream ifs(filename.c_str());
    boost::archive::binary_iarchive ia(ifs);
    //boost::archive::text_iarchive ia(ifs);
    ia >> m;
}
