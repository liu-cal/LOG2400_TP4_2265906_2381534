#pragma once
#include "point.h"
#include "cloud.h"

using namespace std;

const int LARGEUR = 30;
const int HAUTEUR = 20;

class DisplayStrategy
{
public:
    virtual ~DisplayStrategy() = default;
    virtual void draw(
        const vector<Point> &points,
        const vector<Cloud> &clouds,
        const vector<vector<int>> &surfaces,
        const class TextureManager &tm) = 0;
};

class DisplayTextures : public DisplayStrategy
{
public:
    void draw(
        const vector<Point> &points,
        const vector<Cloud> &clouds,
        const vector<vector<int>> &surfaces,
        const class TextureManager &tm) override;
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
