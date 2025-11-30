#pragma once
#include "point.h"
#include "cloud.h"
#include "grid.h"
#include <vector>

using namespace std;

class TextureManager;

class DisplayStrategy
{
public:
    virtual ~DisplayStrategy() = default;
    virtual void draw(
        const vector<Point> &points,
        const vector<Cloud> &clouds,
        const vector<vector<int>> &surfaces,
        const TextureManager &tm) = 0;
};

class DisplayTextures : public DisplayStrategy
{
public:
    void draw(
        const vector<Point> &points,
        const vector<Cloud> &clouds,
        const vector<vector<int>> &surfaces,
        const TextureManager &tm) override;
};

class DisplayIDs : public DisplayStrategy
{
public:
    void draw(
        const vector<Point> &points,
        const vector<Cloud> &clouds,
        const vector<vector<int>> &surfaces,
        const TextureManager &tm) override;
};
