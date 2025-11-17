#pragma once
#include <vector>
#include <memory>
#include "point.h"
#include "cloud.h"
#include "textureManager.h"
#include "display.h"
#include "surface.h"

class Scene
{
public:
    Scene() = default;

    void initFromArgsString(const std::string &args);

    void cmd_list();
    void cmd_display(std::unique_ptr<DisplayStrategy> dsp);
    void cmd_merge_createCloud(const std::vector<int> &ids);
    bool cmd_movePoint(int id, int nx, int ny);
    bool cmd_deletePoint(int id);
    void cmd_buildSurface(std::unique_ptr<SurfaceBuilder> builder);

    const std::vector<Point> &getPoints() const { return points; }
    const std::vector<Cloud> &getClouds() const { return clouds; }
    TextureManager &getTextureManager() { return tm; }

private:
    std::vector<Point> points;
    std::vector<Cloud> clouds;
    std::vector<std::vector<int>> surfaces;
    TextureManager tm;
    int nextCloudId = 0;
};
