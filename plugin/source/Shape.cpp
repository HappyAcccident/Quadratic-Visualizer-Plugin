#define _USE_MATH_DEFINES

#include <iostream>

#include <vector>
#include <cmath>
#include <numeric>
#include <memory>
#include "VisualizerPlugin/Shape.h"

// for regular polygons, steepness is 1. for more info on this function, watch the video https://www.youtube.com/watch?v=AoOv6bWg9lk
// skip to 12:54 to see how this formula also generates stars
float regularPolygonMag(float theta, float n, float phi, int res)
{
    float mag = cos(M_PI/n)/cos((theta-phi) - (2*M_PI/n)*floor((n*(theta-phi) + M_PI)/(2*M_PI)));
    return mag;
}

Shape::Shape(float inputStep, float n, float phi, float outputStep, int res, std::string type)
{
    float theta = 0;
    float thetaStep = 2*M_PI/res;
    if (type == "Regular")
    {
        for (int i = 0; i < res; i++)
        {
            float mag = regularPolygonMag(theta*inputStep, n, phi, res);
            pts.push_back(std::complex<float>(mag*cos(theta*outputStep), mag*sin(theta*outputStep)));
            theta += thetaStep;
        }
    }

    initPts = pts;
    maxRadius = 1;
    initMaxRadius = maxRadius;
}

Shape Shape::operator+(const Shape& other) const
{
    std::vector<std::complex<float>> thisPts = this->pts;
    std::vector<std::complex<float>> otherPts = other.pts;
    std::vector<std::complex<float>> sumPts;

    for (int i = 0; i < thisPts.size(); i++)
    {
        sumPts.push_back(thisPts[i] + otherPts[i]);
    }

    return Shape(sumPts);
}

Shape Shape::operator-(const Shape& other) const
{
    std::vector<std::complex<float>> thisPts = this->pts;
    std::vector<std::complex<float>> otherPts = other.pts;
    std::vector<std::complex<float>> sumPts;

    for (int i = 0; i < thisPts.size(); i++)
    {
        sumPts.push_back(thisPts[i] - otherPts[i]);
    }

    return Shape(sumPts);
}

Shape Shape::operator*(const Shape& other) const
{
    std::vector<std::complex<float>> thisPts = this->pts;
    std::vector<std::complex<float>> otherPts = other.pts;
    std::vector<std::complex<float>> productPts;

    for (int i = 0; i < thisPts.size(); i++)
    {
        productPts.push_back(thisPts[i] * otherPts[i]);
    }

    return Shape(productPts);
}


Shape Shape::operator/(const Shape& other) const
{
    std::vector<std::complex<float>> thisPts = this->pts;
    std::vector<std::complex<float>> otherPts = other.pts;
    std::vector<std::complex<float>> quotientPts;

    for (int i = 0; i < thisPts.size(); i++)
    {
        quotientPts.push_back(thisPts[i] / otherPts[i]);
    }

    return Shape(quotientPts);
}

Shape Shape::sqrt(const Shape& shape)
{
    std::vector<std::complex<float>> sqrtPts;

    for (auto& pt : shape.pts)
    {
        sqrtPts.push_back(std::sqrt(pt));
    }

    return Shape(sqrtPts);
} 

std::pair<Shape, Shape> Shape::quadratic(const Shape& A, const Shape& B, const Shape& C)
{
    std::vector<std::complex<float>> posRootPts;
    std::vector<std::complex<float>> negRootPts;

    auto APts = A.pts;
    auto BPts = B.pts;
    auto CPts = C.pts;

    for (int i = 0; i < APts.size(); i++)
    {
        auto a = APts[i];
        auto b = BPts[i];
        auto c = CPts[i];
        auto negOne = std::complex<float>(-1, 0);
        auto two = std::complex<float>(2, 0);
        auto four = std::complex<float>(4, 0);

        if (a == std::complex<float>(0,0))
        {
            posRootPts.push_back(std::complex<float>(0, 0));
            negRootPts.push_back(std::complex<float>(0, 0));
        }
        else
        {
            auto posRoot = (negOne * b + std::sqrt(b*b - four*a*c))/(two * a);
            auto negRoot = c / (a * posRoot);
            posRootPts.push_back(posRoot);
            negRootPts.push_back(negRoot);
        }
    }

    return std::make_pair<Shape, Shape>(Shape(posRootPts), Shape(negRootPts));
}

void Shape::rotate(float phi)
{
    std::complex<float> rotation (cos(phi), sin(phi));

    for (auto& pt : this->pts)
    {
        pt *= rotation;
    }
}

void Shape::addAnimation(Animation* animation) 
{
    animations.push_back(animation);
}

//inserts animation at the orderth index in the animations vector
void Shape::addAnimation(Animation* animation, const int& order)
{
    animations.insert(animations.begin() + order, animation);
}

void Shape::updateAnimations(const State& currentState)
{
    auto newPts = this->getInitPts();
    auto newMaxRadius = 0;
    for (auto& pt : newPts)
    {
        for (Animation* animation : animations)
        {
            animation->makeTransformation(currentState);
            pt = animation->getTransformation()(pt);
        }

        float mag = abs(sqrtf(pt.real() * pt.real() + pt.imag() * pt.imag()));
        if (mag > newMaxRadius)
        {
            newMaxRadius = mag;
        }
    }
    this->setPts(newPts);
    this->setMaxRadius(newMaxRadius);
}
