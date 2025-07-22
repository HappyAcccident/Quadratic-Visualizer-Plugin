#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <complex>
#include "State.h"

class Animation {
protected:
    std::vector<std::complex<float>> initPts;
    float initMaxRadius;
public:
    Animation() {}

    virtual void addToShape(const std::vector<std::complex<float>>& pts, const float& maxRadius, const State& currentState) = 0;
    virtual std::pair<std::vector<std::complex<float>>, float> newPts(const State& currentState) const = 0;
};

class FrameAnimation : public Animation 
{
protected:
    int initFrame;
private:
    void addToShape(const std::vector<std::complex<float>>& pts, const float& maxRadius, const State& currentState) {initPts = pts; initMaxRadius = maxRadius; initFrame = currentState.getFrameCounter();}
};

class ScaleAnimation : public FrameAnimation
{
protected:
    float frequency;
    float min;
    float max;
public:
    ScaleAnimation(float frequency, float min, float max) : frequency(frequency), min(min), max(max) {}

    std::pair<std::vector<std::complex<float>>, float> newPts(const State& currentState) const override;
};

class RotateAnimation : public FrameAnimation
{
protected:
    float frequency;
public:
    RotateAnimation(float frequency) : frequency(frequency) {}

    std::pair<std::vector<std::complex<float>>, float> newPts(const State& currentState) const override;
};

class VolumeAnimation : public Animation
{
protected:
    Band bandType;
public:
    void addToShape(const std::vector<std::complex<float>>& pts, const float& maxRadius, const State& currentState) {initPts = pts; initMaxRadius = maxRadius;}
    VolumeAnimation(Band bandType) : bandType(bandType) {}

    std::pair<std::vector<std::complex<float>>, float> newPts(const State& currentState) const override;
};

#endif