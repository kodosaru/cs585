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
    private:
        int frameNo;
        int fps;
        float offset;
        cv::Point circleCenter;
        float radius;

    public:
        void setCircle(cv::Point circleCenter, float radius);
        void setFrame(int fps, int frameNo);
        std::string map2json (const std::map<std::string, std::string>& map);
        Tracking();
        Tracking(int fps, int frameNo);
};
#endif /* defined(__Prototype__Tracking__) */
