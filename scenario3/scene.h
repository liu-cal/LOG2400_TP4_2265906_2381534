#pragma once
#include <vector>
#include <memory>
#include "point.h"
#include "cloud.h"
#include "textureManager.h"
#include "display.h"
#include "surface.h"
#include "command.h"

using namespace std;

class Scene
{
public:
    Scene() = default;

    void initFromArgsString(const string &args);

    void cmd_list();
    void cmd_display(unique_ptr<DisplayStrategy> dsp);
    void cmd_merge_createCloud(const vector<int> &ids);
    bool cmd_movePoint(int id, int nx, int ny);
    bool cmd_deletePoint(int id);
    void cmd_buildSurface(unique_ptr<SurfaceBuilder> builder);

    void undo();
    void redo();

    const vector<Point> &getPoints() const { return points; }
    const vector<Cloud> &getClouds() const { return clouds; }
    TextureManager &getTextureManager() { return tm; }

    vector<Point> &getPointsMutable() { return points; }
    vector<Cloud> &getCloudsMutable() { return clouds; }

private:
    vector<Point> points;
    vector<Cloud> clouds;
    vector<vector<int>> surfaces;
    TextureManager tm;
    int nextCloudId = 0;

    CommandManager commandManager;

    int globalIdToCloudIndex(int globalId) const
    {
        return globalId - static_cast<int>(points.size());
    }

    vector<int> getAllPointsInCloud(int cloudId) const;
};
