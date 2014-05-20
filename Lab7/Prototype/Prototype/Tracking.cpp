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

void Tracking::set(int frameNo, float offset, int radius, cv::Point circleCenter)
{
    this->frameNo=frameNo;
    this->offset=offset;
    this->radius=radius;
    this->circleCenter=circleCenter;
};

std::string Tracking::map2json (const std::map<std::string, std::string>& map) {
    ptree pt;
    for (auto& entry: map)
        pt.put (entry.first, entry.second);
    std::ostringstream buf;
    write_json (buf, pt, false);
    return buf.str();
}