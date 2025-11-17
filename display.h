#pragma once
#include <vector>
#include "point.h"
#include "cloud.h"

const int LARGEUR = 30;
const int HAUTEUR = 20;

class DisplayStrategy
{
public:
    virtual ~DisplayStrategy() = default;
    virtual void draw(
        const std::vector<Point> &points,
        const std::vector<Cloud> &clouds,
        const std::vector<std::vector<int>> &surfaces,
        const class TextureManager &tm) = 0;
};

class DisplayTextures : public DisplayStrategy
{
public:
    void draw(
        const std::vector<Point> &points,
        const std::vector<Cloud> &clouds,
        const std::vector<std::vector<int>> &surfaces,
        const class TextureManager &tm) override;
};

class DisplayIDs : public DisplayStrategy
{
public:
    void draw(
        const std::vector<Point> &points,
        const std::vector<Cloud> &clouds,
        const std::vector<std::vector<int>> &surfaces,
        const TextureManager &tm) override;
};
