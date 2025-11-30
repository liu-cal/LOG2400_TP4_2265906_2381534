#include "scene.h"
#include "command.h"
#include <sstream>
#include <iostream>
#include <algorithm>

void Scene::initFromArgsString(const string &args)
{
   istringstream iss(args);
   string token;
    int idCounter = 0;
    while (iss >> token)
    {
        if (token.size() >= 3 && token.front() == '(' && token.back() == ')')
        {
            token = token.substr(1, token.size() - 2);
           replace(token.begin(), token.end(), ',', ' ');
           istringstream pair(token);
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
   cout << "Liste:\n";
    for (const auto &p : points)
    {
        if (!p.active)
            continue;
       cout << p.id << ": " << "(" << p.x << "," << p.y << ") textures: '" << p.texture << "'\n";
    }

    if (!clouds.empty())
    {
        for (const auto &c : clouds)
        {
           cout << c.id << ": Nuage '" << tm.get(c.textureIndex) << "' contient les points: ";
            for (size_t i = 0; i < c.pointIds.size(); i++)
            {
                if (i == c.pointIds.size() - 1)
                {
                   cout << c.pointIds[i];
                }
                else
                {
                   cout << c.pointIds[i] << ", ";
                }
            }
           cout << "\n";
        }
    }
}

void Scene::cmd_display(unique_ptr<DisplayStrategy> dsp)
{
    dsp->draw(points, clouds, surfaces, tm);
}

void Scene::cmd_merge_createCloud(const vector<int> &ids)
{
   vector<int> texturePoints;
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

   vector<int> uniq;
    uniq.reserve(texturePoints.size());
    for (int p : texturePoints)
    {
        if (find(uniq.begin(), uniq.end(), p) == uniq.end())
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

    auto command =make_unique<MoveCommand>(this, id, nx, ny);
    commandManager.executeCommand(move(command));
    return true;
}

bool Scene::cmd_deletePoint(int id)
{
    if (id < 0 || id >= (int)points.size() || !points[id].active)
        return false;

    auto command =make_unique<DeleteCommand>(this, id);
    commandManager.executeCommand(move(command));
    return true;
}

vector<int> Scene::getAllPointsInCloud(int cloudId) const
{
   vector<int> result;
    if (cloudId < 0)
        return result;

    int cindex = globalIdToCloudIndex(cloudId);
    if (cindex < 0 || cindex >= static_cast<int>(clouds.size()))
    {
        return result;
    }

    const auto &c = clouds[cindex];
    for (int pid : c.pointIds)
    {
        if (pid < static_cast<int>(points.size()))
        {
            if (points[pid].active)
            {
                result.push_back(pid);
            }
        }
        else
        {
            int cloudIndex = globalIdToCloudIndex(pid);
            if (cloudIndex >= 0 && cloudIndex < static_cast<int>(clouds.size()))
            {
                auto subPoints = getAllPointsInCloud(clouds[cloudIndex].id);
                result.insert(result.end(), subPoints.begin(), subPoints.end());
            }
        }
    }

   sort(result.begin(), result.end());
    result.erase(unique(result.begin(), result.end()), result.end());

    return result;
}

void Scene::cmd_buildSurface(unique_ptr<SurfaceBuilder> builder)
{
    surfaces.clear();

    for (const auto &c : clouds)
    {
        auto allPointsInCloud = getAllPointsInCloud(c.id);

        if (!allPointsInCloud.empty())
        {
            Cloud tempCloud(-1);
            tempCloud.pointIds = allPointsInCloud;

            auto poly = builder->build(tempCloud, points);
            if (!poly.empty())
            {
                surfaces.push_back(poly);
            }
        }
    }
}

void Scene::undo()
{
    commandManager.undo();
}

void Scene::redo()
{
    commandManager.redo();
}