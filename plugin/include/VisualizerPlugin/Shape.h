#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <complex>
#include "Animation.h"

class Shape {
protected:
    std::vector<std::complex<float>> initPts;
    float initMaxRadius = 0;

    std::vector<std::complex<float>> pts; //complex numbers
    std::vector<bool> isVertex;
    float maxRadius = 0; //gives maximum magnitude of points, used to scale display
    std::vector<Animation*> animations;

    float inputStep;
    float n;
    float phi;
    float outputStep;
    int resolution;
    std::string type;
public:
    // set of points without a known max radius
    Shape(std::vector<std::complex<float>> pts) : pts(pts), resolution(pts.size())
    {
        float maxMag = 0.0;
        for (auto pt : pts)
        {
            double real = pt.real();
            double imag = pt.imag();
            double mag = sqrtf(real*real + imag*imag);
            if (mag > maxMag)
            {
                maxMag = mag;
            }
        }
        maxRadius = maxMag;
    }

    //if you want to make a circle, set vertices equal to 0
    //if you want to make a spiral, set vertices equal to -1
    Shape(float inputStep, float n, float phi, float outputStep, int res, std::string type);
    Shape() {}

    void regenerate();

    float getInputStep() const { return inputStep; }
    float getN() const { return n; }
    float getPhi() const { return phi; }
    float getOutputStep() const { return outputStep; }
    int getResolution() const { return resolution; }
    const std::string& getType() const { return type; }

    void setInputStep(float value) { inputStep = value; }
    void setN(float value) { n = value; }
    void setPhi(float value) { phi = value; }
    void setOutputStep(float value) { outputStep = value; }
    void setResolution(int value) { resolution = value; }
    void setType(const std::string& value) { type = value; }

    std::vector<std::complex<float>> getPts() const {return pts;}
    std::vector<std::complex<float>> getInitPts() const {return initPts;}
    float getInitMaxRadius() const {return initMaxRadius;}

    float getMaxRadius() const {return maxRadius;}

    void setPts(const std::vector<std::complex<float>> newPts) {pts = newPts;}
    void setMaxRadius(float newMaxRadius) {maxRadius = newMaxRadius;}

    Shape operator+(const Shape& other) const;
    Shape operator-(const Shape& other) const;
    Shape operator*(const Shape& other) const;
    Shape operator/(const Shape& other) const;

    static Shape sqrt(const Shape& shape); //using principle branch of complex log
    static std::pair<Shape, Shape> quadratic(const Shape& A, const Shape& B, const Shape& C);

    void rotate(float phi);
    void scale(float scalar);

    void addAnimation(Animation* animation);
    void addAnimation(Animation* animation, const int& order); 
    void updateAnimations(const State& currentState);
};

#endif