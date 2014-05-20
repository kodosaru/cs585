//
//  Tracking.cpp
//  Prototype
//
//  Created by Don Johnson on 5/20/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#include "Tracking.hpp"
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

std::string Tracking::map2json (const std::map<std::string, std::string>& map) {
    ptree pt;
    for (auto& entry: map)
        pt.put (entry.first, entry.second);
    std::ostringstream buf;
    write_json (buf, pt, false);
    return buf.str();
}

void Tracking::setFrame(int fps, int frameNo)
{
    if(fps <= 0)
    {
        std::cout<<"Error in Tracking::setFrameNo - frames per seconds <= 0"<<std::endl;
        frameNo = -1;
        return;
    }
    this->fps=fps;
    this->frameNo=frameNo;
    this->offset=1.0/fps * frameNo;
};

void Tracking::setCircle(cv::Point circleCenter, float radius)
{
    this->radius=radius;
    this->circleCenter=circleCenter;
};

Tracking::Tracking()
{
    this->fps = -1;
    this->frameNo = -1;
    this->offset = 0.0;
    this->circleCenter = cv::Point(-1,-1);
    this->radius = -1.0;
}

Tracking::Tracking(int fps, int frameNo)
{
    this->fps = fps;
    this->frameNo = frameNo;
    this->offset = 0.0;
    this->circleCenter = cv::Point(-1,-1);
    this->radius = -1.0;
}