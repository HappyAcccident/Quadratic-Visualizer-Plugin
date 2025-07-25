#define _USE_MATH_DEFINES

#include <cmath>
#include "VisualizerPlugin/Animation.h"

using Transformation = std::function<std::complex<float>(const std::complex<float>&)>;

void ScaleAnimation::makeTransformation(const State& currentState)
{
    float scalar = (max-min)*(sinf(2*M_PI*(currentState.getFrameCounter() - (frequency)/2)/frequency))/2 + (max+min)/2;
    transformation = [scalar](std::complex<float> pt) -> std::complex<float> {return pt * scalar;};
}

void RotateAnimation::makeTransformation(const State& currentState)
{
    float phi = 2*M_PI*currentState.getFrameCounter()/frequency;
    std::complex<float> rotation (cos(phi), sin(phi));
    transformation = [rotation](std::complex<float> pt) -> std::complex<float> {return pt * rotation;};
};

void VolumeScaleAnimation::makeTransformation(const State& currentState)
{
    float scalar = calculateScalar(currentState.getMeanBandVolume(bandType));
    transformation = [scalar](std::complex<float> pt) -> std::complex<float> {return pt * scalar;};
};

void VolumeRotateAnimation::makeTransformation(const State& currentState)
{
    float phi = M_PI*currentState.getMeanBandVolume(bandType)*256.0f/180.0f;
    std::complex<float> rotation (cos(phi), sin(phi));
    transformation = [rotation](std::complex<float> pt) -> std::complex<float> {return pt * rotation;};
};