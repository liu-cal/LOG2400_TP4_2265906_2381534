#include "scene.h"
#include <sstream>
#include <iostream>
#include <algorithm>

void Scene::initFromArgsString(const std::string &ligne)
{
    std::istringstream iss(ligne);
    std::string token;
    int idCounter = 0;
    while (iss >> token)
    {
        if (token.size() >= 3 && token.front() == '(' && token.back() == ')')
        {
            token = token.substr(1, token.size() - 2);
            std::replace(token.begin(), token.end(), ',', ' ');
            std::istringstream pair(token);
            int x, y;
            if (pair >> x >> y)
            {
                points.emplace_back(idCounter++, x, y);
            }
        }
    }
}

void Scene::cmd_list()
{
    std::cout << "Liste:\n";
    for (const auto &p : points)
    {
        if (!p.active)
            continue;
        std::cout << p.id << ": " << "(" << p.x << "," << p.y << ") textures: '" << p.texture << "'\n";
    }

    if (clouds.size() != 0)
    {
        std::cout << "\nNuages:\n";
        for (const auto &c : clouds)
        {
            std::cout << "CloudID=" << c.id << " textureIndex=" << c.textureIndex << " pts:";
            for (int pid : c.pointIds)
                std::cout << pid << " ";
            std::cout << "\n";
        }
    }
}

void Scene::cmd_display(std::unique_ptr<DisplayStrategy> dsp)
{
    dsp->draw(points, clouds, tm);
}

void Scene::cmd_merge_createCloud(const std::vector<int> &ids)
{
    Cloud c(nextCloudId++);
    c.pointIds = ids;
    c.textureIndex = clouds.size() % (tm.count() ? tm.count() : 1);

    for (int pid : ids)
    {
        if (pid >= 0 && pid < (int)points.size())
            points[pid].texture = tm.get(c.textureIndex);
    }
    clouds.push_back(c);
    std::cout << "Cloud créé id=" << c.id << " avec " << c.pointIds.size() << " points, texture '" << tm.get(c.textureIndex) << "'\n";
}

bool Scene::cmd_movePoint(int id, int nx, int ny)
{
    if (id < 0 || id >= (int)points.size() || !points[id].active)
        return false;
    points[id].x = nx;
    points[id].y = ny;
    return true;
}

bool Scene::cmd_deletePoint(int id)
{
    if (id < 0 || id >= (int)points.size() || !points[id].active)
        return false;
    points[id].active = false;

    for (auto &c : clouds)
    {
        c.pointIds.erase(std::remove(c.pointIds.begin(), c.pointIds.end(), id), c.pointIds.end());
    }
    return true;
}

void Scene::cmd_buildSurface(std::unique_ptr<SurfaceBuilder> builder)
{
    for (const auto &c : clouds)
    {
        auto poly = builder->build(c, points);
        std::cout << "Surface pour Cloud " << c.id << " : ";
        for (int pid : poly)
            std::cout << pid << " ";
        std::cout << "\n";
    }
}
