#pragma once
#include <vector>
#include "cloud.h"
#include "point.h"

using namespace std;

class SurfaceBuilder
{
public:
    virtual ~SurfaceBuilder() = default;
    virtual vector<int> build(const Cloud &cloud, const vector<Point> &allPoints) = 0;
};

class SurfaceByIDOrder : public SurfaceBuilder
{
public:
    vector<int> build(const Cloud &cloud, const vector<Point> &allPoints) override;
};

class SurfaceByNearest : public SurfaceBuilder
{
public:
    vector<int> build(const Cloud &cloud, const vector<Point> &allPoints) override;
};