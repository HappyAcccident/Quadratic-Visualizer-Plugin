#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <complex>
#include "State.h"


using Transformation = std::function<std::complex<float>(const std::complex<float>&)>;

class Animation {
protected:
    Transformation transformation;
public:
    Animation() {}
    Animation(Transformation transformation) : transformation(transformation) {}
    const Transformation& getTransformation() const {return transformation;}

    virtual void makeTransformation(const State& currentState) = 0;
};

class FrameAnimation : public Animation 
{
protected:
    int initFrame;
};

class ScaleAnimation : public FrameAnimation
{
protected:
    float frequency;
    float min;
    float max;
public:
    ScaleAnimation(float frequency, float min, float max) : frequency(frequency), min(min), max(max) {}

    void makeTransformation(const State& currentState) override;
};

class RotateAnimation : public FrameAnimation
{
protected:
    float frequency;
public:
    RotateAnimation(float frequency) : frequency(frequency) {}

    void makeTransformation(const State& currentState) override;
};

class VolumeAnimation : public Animation
{
protected:
    Band bandType;
public:
    VolumeAnimation(Band bandType) : bandType(bandType) {}
};

class VolumeScaleAnimation : public VolumeAnimation
{
public:
    using ScalarFunc = std::function<float(const float&)>;

    //bandType specifies the meanBandVolume the scalarFunc will act on
    //scalarFunc defines the scalar each point is multiplied by using the meanBandVolume
    VolumeScaleAnimation(Band bandType, ScalarFunc scalarFunc) : VolumeAnimation(bandType), calculateScalar(scalarFunc) {}

    void makeTransformation(const State& currentState) override;
private:
    ScalarFunc calculateScalar;
};

class VolumeRotateAnimation : public VolumeAnimation
{
public:
    VolumeRotateAnimation(Band bandType) : VolumeAnimation(bandType) {}
    void makeTransformation(const State& currentState) override;
};

#endif