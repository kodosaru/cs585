//
//  Tracking.h
//  Prototype
//
//  Created by Don Johnson on 5/20/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#ifndef __Prototype__Tracking__
#define __Prototype__Tracking__

#include <iostream>
#include <opencv2/core/core.hpp>


class Tracking
{
    float offset;
    int radius;
    cv::Point circleCenter;
    int fps;
    public:
        void set(float offset, int radius, cv::Point circleCenter);
        std::string map2json (const std::map<std::string, std::string>& map);
    float vidOffset(int fps, );
};
#endif /* defined(__Prototype__Tracking__) */
