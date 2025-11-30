#include "display.h"
#include "textureManager.h"

using namespace std;

void DisplayTextures::draw(
    const vector<Point> &points,
    const vector<Cloud> &clouds,
    const vector<vector<int>> &surfaces,
    const TextureManager &tm)
{
    Grid grid;
    
    vector<string> mapTexture(points.size(), "");
    for (const auto &p : points)
    {
        if (p.id >= 0 && p.id < (int)points.size())
            mapTexture[p.id] = p.texture;
    }

    for (const auto &p : points)
    {
        if (!p.active)
            continue;
        
        string content = (mapTexture[p.id] == "" ? "." : mapTexture[p.id]);
        grid.setCell(p.x, p.y, content);
    }

    grid.print();
}

void DisplayIDs::draw(
    const vector<Point> &points,
    const vector<Cloud> &clouds,
    const vector<vector<int>> &surfaces,
    const TextureManager &tm)
{
    Grid grid;
    
    for (const auto &p : points)
    {
        if (!p.active)
            continue;
        
        char ch = (p.id < 10 ? char('0' + p.id) : '*');
        grid.setCell(p.x, p.y, ch);
    }

    grid.traceSurfaces(surfaces, points);
    grid.print();
}
