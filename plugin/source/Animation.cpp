#define _USE_MATH_DEFINES

#include <cmath>
#include "VisualizerPlugin/Animation.h"


std::pair<std::vector<std::complex<float>>, float> ScaleAnimation::newPts(const State& currentState) const
{
    std::vector<std::complex<float>> pts;
    float maxRadius;
    float scalar = (max-min)*(sinf(2*M_PI*(currentState.getFrameCounter() - (frequency)/2)/frequency))/2 + (max+min)/2;
    for (auto pt : initPts)
    {
        pts.push_back(pt * scalar);
    }
    maxRadius = initMaxRadius*scalar;
    return std::make_pair(pts, maxRadius);
};

std::pair<std::vector<std::complex<float>>, float> RotateAnimation::newPts(const State& currentState) const
{
    std::vector<std::complex<float>> pts;
    float phi = 2*M_PI*currentState.getFrameCounter()/frequency;
    std::complex<float> rotation (cos(phi), sin(phi));
    for (auto pt : initPts)
    {
        pts.push_back(pt * rotation);
    }
    return std::make_pair(pts, initMaxRadius);
};

std::pair<std::vector<std::complex<float>>, float> VolumeScaleAnimation::newPts(const State& currentState) const
{
    std::vector<std::complex<float>> pts;
    float maxRadius;
    float scalar = calculateScalar(currentState.getMeanBandVolume(bandType));
    for (auto pt : initPts)
    {
        pts.push_back(pt * scalar);
    }
    maxRadius = initMaxRadius*scalar;
    return std::make_pair(pts, maxRadius);
};

std::pair<std::vector<std::complex<float>>, float> VolumeRotateAnimation::newPts(const State& currentState) const
{
    std::vector<std::complex<float>> pts;
    float phi = M_PI*currentState.getMeanBandVolume(bandType)/180.0f;
    std::complex<float> rotation (cos(phi), sin(phi));
    for (auto pt : initPts)
    {
        pts.push_back(pt * rotation);
    }
    return std::make_pair(pts, initMaxRadius);
};