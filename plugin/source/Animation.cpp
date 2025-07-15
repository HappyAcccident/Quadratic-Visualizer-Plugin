#define _USE_MATH_DEFINES

#include <cmath>
#include "VisualizerPlugin/Animation.h"


std::pair<std::vector<std::complex<float>>, float> ScaleAnimation::newPts(int frame)
{
    std::vector<std::complex<float>> pts;
    float maxRadius;
    float scalar = (max-min)*(sinf(2*M_PI*(frame - (frequency)/2)/frequency))/2 + (max+min)/2;
    for (auto pt : initPts)
    {
        pts.push_back(pt * scalar);
    }
    maxRadius = initMaxRadius*scalar;
    return std::make_pair(pts, maxRadius);
};

std::pair<std::vector<std::complex<float>>, float> RotateAnimation::newPts(int frame)
{
    std::vector<std::complex<float>> pts;
    float phi = 2*M_PI*frame/frequency;
    std::complex<float> rotation (cos(phi), sin(phi));
    for (auto pt : initPts)
    {
        pts.push_back(pt * rotation);
    }
    return std::make_pair(pts, initMaxRadius);
};