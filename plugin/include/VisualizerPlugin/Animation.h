#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <complex>

class Animation {
protected:
    std::vector<std::complex<float>> initPts;
    int initFrame;
    float initMaxRadius;
public:
    Animation() {}

    void addToShape(std::vector<std::complex<float>> pts, float maxRadius, int frame) {initPts = pts; initMaxRadius = maxRadius; initFrame = frame;}

    virtual std::pair<std::vector<std::complex<float>>, float> newPts(int frame) = 0;
    virtual std::pair<std::vector<std::complex<float>>, float> newPts(float volume) = 0;
};

class ScaleAnimation : public Animation
{
protected:
    float frequency;
    float min;
    float max;
public:
    ScaleAnimation(float frequency, float min, float max) : frequency(frequency), min(min), max(max) {}

    std::pair<std::vector<std::complex<float>>, float> newPts(int frame) override;
    std::pair<std::vector<std::complex<float>>, float> newPts(float volume) override;
};

class RotateAnimation : public Animation
{
protected:
    float frequency;
public:
    RotateAnimation(float frequency) : frequency(frequency) {}

    std::pair<std::vector<std::complex<float>>, float> newPts(int frame) override;
    std::pair<std::vector<std::complex<float>>, float> newPts(float volume) override;
};

class VolumeAnimation : public Animation
{
protected:
public:
    VolumeAnimation() {}

    std::pair<std::vector<std::complex<float>>, float> newPts(int frame) override;
    std::pair<std::vector<std::complex<float>>, float> newPts(float volume) override;
};

#endif