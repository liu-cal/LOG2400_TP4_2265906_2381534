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

    nextCloudId = static_cast<int>(points.size());
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

    if (!clouds.empty())
    {
        for (const auto &c : clouds)
        {
            std::cout << c.id << ": Nuage '" << tm.get(c.textureIndex) << "' contient les points: ";
            for (size_t i = 0; i < c.pointIds.size(); i++)
            {
                if (i == c.pointIds.size() - 1)
                {
                    std::cout << c.pointIds[i];
                }
                else
                {
                    std::cout << c.pointIds[i] << ", ";
                }
            }
            std::cout << "\n";
        }
    }
}

void Scene::cmd_display(std::unique_ptr<DisplayStrategy> dsp)
{
    dsp->draw(points, clouds, surfaces, tm);
}

void Scene::cmd_merge_createCloud(const std::vector<int> &ids)
{
    std::vector<int> texturePoints;
    texturePoints.reserve(ids.size() * 2);

    for (int gid : ids)
    {
        if (gid < 0)
        {
            continue;
        }
        if (gid < static_cast<int>(points.size()))
        {
            if (points[gid].active)
            {
                texturePoints.push_back(gid);
            }
        }
        else
        {
            int cindex = globalIdToCloudIndex(gid);
            if (cindex >= 0 && cindex < static_cast<int>(clouds.size()))
            {
                texturePoints.push_back(gid);
            }
        }
    }

    std::vector<int> uniq;
    uniq.reserve(texturePoints.size());
    for (int p : texturePoints)
    {
        if (std::find(uniq.begin(), uniq.end(), p) == uniq.end())
        {
            uniq.push_back(p);
        }
    }

    Cloud newCloud(nextCloudId++);
    newCloud.pointIds = uniq;
    newCloud.textureIndex = clouds.size() % (tm.count() ? tm.count() : 1);

    char textChar = tm.get(newCloud.textureIndex);
    for (int pid : uniq)
    {
        std::cout << pid << " ";
        if (pid >= 0 && pid < static_cast<int>(points.size()))
        {
            points[pid].texture += textChar;
        }
        else
        {
            int cindex = globalIdToCloudIndex(pid);
            for (int pid : clouds[cindex].pointIds)
            {
                if (points[pid].active)
                {
                    points[pid].texture += textChar;
                }
            }
        }
    }

    clouds.push_back(newCloud);
}

bool Scene::cmd_movePoint(int id, int nx, int ny)
{
    if (id < 0 || id >= (int)points.size() || !points[id].active)
        return false;

    Action act;
    act.type = Action::MOVE;
    act.pointId = id;
    act.oldX = points[id].x;
    act.oldY = points[id].y;
    act.newX = nx;
    act.newY = ny;

    points[id].x = nx;
    points[id].y = ny;

    undoStack.push_back(act);
    redoStack.clear();
    return true;
}

bool Scene::cmd_deletePoint(int id)
{
    if (id < 0 || id >= (int)points.size() || !points[id].active)
        return false;

    Action act;
    act.type = Action::DELETE;
    act.pointId = id;
    act.deletedPoint = points[id];
    act.cloudsWithPoint.clear();

    points[id].active = false;

    for (size_t ci = 0; ci < clouds.size(); ++ci)
    {
        auto &c = clouds[ci];
        auto it = std::find(c.pointIds.begin(), c.pointIds.end(), id);
        if (it != c.pointIds.end())
        {
            act.cloudsWithPoint.push_back(static_cast<int>(ci));
            c.pointIds.erase(std::remove(c.pointIds.begin(), c.pointIds.end(), id), c.pointIds.end());
        }
    }

    undoStack.push_back(act);
    redoStack.clear();

    return true;
}

void Scene::cmd_buildSurface(std::unique_ptr<SurfaceBuilder> builder)
{
    surfaces.clear();

    for (const auto &c : clouds)
    {
        auto poly = builder->build(c, points);
        if (!poly.empty())
        {
            surfaces.push_back(poly);
        }
    }
}

void Scene::undo()
{
    if (undoStack.empty())
    {
        return;
    }

    Action act = undoStack.back();
    undoStack.pop_back();

    if (act.type == Action::MOVE)
    {
        if (act.pointId >= 0 && act.pointId < static_cast<int>(points.size()))
        {
            points[act.pointId].x = act.oldX;
            points[act.pointId].y = act.oldY;
        }
    }
    else if (act.type == Action::DELETE)
    {
        if (act.pointId >= 0 && act.pointId < static_cast<int>(points.size()))
        {
            points[act.pointId] = act.deletedPoint;
            points[act.pointId].active = true;

            for (int ci : act.cloudsWithPoint)
            {
                if (ci >= 0 && ci < static_cast<int>(clouds.size()))
                {
                    clouds[ci].pointIds.push_back(act.pointId);
                }
            }
        }
    }

    redoStack.push_back(act);
}

void Scene::redo()
{
    if (redoStack.empty())
    {
        return;
    }

    Action act = redoStack.back();
    redoStack.pop_back();

    if (act.type == Action::MOVE)
    {
        if (act.pointId >= 0 && act.pointId < static_cast<int>(points.size()))
        {
            points[act.pointId].x = act.newX;
            points[act.pointId].y = act.newY;
        }
    }
    else if (act.type == Action::DELETE)
    {
        if (act.pointId >= 0 && act.pointId < static_cast<int>(points.size()))
        {
            points[act.pointId].active = false;

            for (int ci : act.cloudsWithPoint)
            {
                if (ci >= 0 && ci < static_cast<int>(clouds.size()))
                {
                    auto &vec = clouds[ci].pointIds;
                    vec.erase(std::remove(vec.begin(), vec.end(), act.pointId), vec.end());
                }
            }
        }
    }

    undoStack.push_back(act);
}