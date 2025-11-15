#pragma once
#include <vector>
#include "cloud.h"
#include "point.h"

class SurfaceBuilder
{
public:
    virtual ~SurfaceBuilder() = default;
    virtual std::vector<int> build(const Cloud &cloud, const std::vector<Point> &allPoints) = 0;
};

class SurfaceByIDOrder : public SurfaceBuilder
{
public:
    std::vector<int> build(const Cloud &cloud, const std::vector<Point> &allPoints) override;
};

class SurfaceByNearest : public SurfaceBuilder
{
public:
    std::vector<int> build(const Cloud &cloud, const std::vector<Point> &allPoints) override;
};