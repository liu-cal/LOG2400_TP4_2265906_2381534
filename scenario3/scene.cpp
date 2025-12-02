#include "scene.h"
#include "command.h"
#include "node.h"
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

    for (const auto &cloudNode : cloudNodes)
    {
        cout << cloudNode->getId() << ": Nuage '" << tm.get(cloudNode->getTextureIndex()) << "' contient les éléments: ";
        const auto &children = cloudNode->getChildren();
        for (size_t i = 0; i < children.size(); i++)
        {
            if (i == children.size() - 1)
            {
                cout << children[i]->getId();
            }
            else
            {
                cout << children[i]->getId() << " ";
            }
        }
        cout << "\n";
    }
}

void Scene::cmd_display(unique_ptr<DisplayStrategy> dsp)
{
    updateCloudsCache();
    dsp->draw(points, cloudsCache, surfaces, tm);
}

void Scene::cmd_merge_createCloud(const vector<int> &ids)
{
    auto newCloudNode = make_unique<CloudNode>(nextCloudId++);
    newCloudNode->setTextureIndex(cloudNodes.size() % (tm.count() ? tm.count() : 1));
    
    char textChar = tm.get(newCloudNode->getTextureIndex());
    
    for (int gid : ids)
    {
        if (gid < 0)
            continue;
            
        auto node = createNodeFromGlobalId(gid);
        if (node)
        {
            newCloudNode->addChild(move(node));
        }
    }
    
    newCloudNode->applyTexture(textChar, points);
    
    cloudNodes.push_back(move(newCloudNode));
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

void Scene::cmd_buildSurface(unique_ptr<SurfaceBuilder> builder)
{
    surfaces.clear();

    for (const auto &cloudNode : cloudNodes)
    {
        auto allPointsInCloud = cloudNode->getAllPointIds(points);

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

const vector<Cloud> &Scene::getClouds() const
{
    updateCloudsCache();
    return cloudsCache;
}

unique_ptr<Node> Scene::createNodeFromGlobalId(int globalId)
{
    if (globalId < 0)
        return nullptr;
        
    if (globalId < static_cast<int>(points.size()))
    {
        if (points[globalId].active)
        {
            return make_unique<PointNode>(globalId);
        }
    }
    else
    {
        int cindex = globalIdToCloudIndex(globalId);
        if (cindex >= 0 && cindex < static_cast<int>(cloudNodes.size()))
        {
            const CloudNode* existingCloud = cloudNodes[cindex].get();
            if (existingCloud)
            {
                return existingCloud->clone();
            }
        }
    }
    return nullptr;
}

void Scene::updateCloudsCache() const
{
    cloudsCache.clear();
    for (const auto& cloudNode : cloudNodes)
    {
        cloudsCache.push_back(Cloud::fromCloudNode(*cloudNode, points));
    }
}