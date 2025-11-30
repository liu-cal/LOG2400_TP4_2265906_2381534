#include "command.h"
#include "scene.h"
#include <algorithm>

void CommandManager::executeCommand(unique_ptr<ICommand> command)
{
    command->execute();
    undoStack.push_back(move(command));
    redoStack.clear();
}

void CommandManager::undo()
{
    if (undoStack.empty())
        return;

    auto command = move(undoStack.back());
    undoStack.pop_back();
    command->undo();
    redoStack.push_back(move(command));
}

void CommandManager::redo()
{
    if (redoStack.empty())
        return;

    auto command = move(redoStack.back());
    redoStack.pop_back();
    command->redo();
    undoStack.push_back(move(command));
}

MoveCommand::MoveCommand(Scene* scene, int pointId, int newX, int newY)
    : scene(scene), pointId(pointId), oldX(0), oldY(0), newX(newX), newY(newY), executed(false)
{}

void MoveCommand::execute()
{
    if (executed)
        return;

    auto& points = scene->getPointsMutable();
    if (pointId < 0 || pointId >= static_cast<int>(points.size()) || !points[pointId].active)
        return;

    oldX = points[pointId].x;
    oldY = points[pointId].y;
    points[pointId].x = newX;
    points[pointId].y = newY;
    executed = true;
}

void MoveCommand::undo()
{
    if (!executed)
        return;

    auto& points = scene->getPointsMutable();
    if (pointId >= 0 && pointId < static_cast<int>(points.size()))
    {
        points[pointId].x = oldX;
        points[pointId].y = oldY;
    }
}

void MoveCommand::redo()
{
    if (!executed)
        return;

    auto& points = scene->getPointsMutable();
    if (pointId >= 0 && pointId < static_cast<int>(points.size()))
    {
        points[pointId].x = newX;
        points[pointId].y = newY;
    }
}

DeleteCommand::DeleteCommand(Scene* scene, int pointId)
    : scene(scene), pointId(pointId), deletedPoint(-1, 0, 0), executed(false)
{}

void DeleteCommand::execute()
{
    if (executed)
        return;

    auto& points = scene->getPointsMutable();
    auto& clouds = scene->getCloudsMutable();

    if (pointId < 0 || pointId >= static_cast<int>(points.size()) || !points[pointId].active)
        return;

    deletedPoint = points[pointId];
    points[pointId].active = false;

    cloudsWithPoint.clear();
    for (size_t ci = 0; ci < clouds.size(); ++ci)
    {
        auto& c = clouds[ci];
        auto it = find(c.pointIds.begin(), c.pointIds.end(), pointId);
        if (it != c.pointIds.end())
        {
            cloudsWithPoint.push_back(static_cast<int>(ci));
            c.pointIds.erase(remove(c.pointIds.begin(), c.pointIds.end(), pointId), c.pointIds.end());
        }
    }

    executed = true;
}

void DeleteCommand::undo()
{
    if (!executed)
        return;

    auto& points = scene->getPointsMutable();
    auto& clouds = scene->getCloudsMutable();

    if (pointId >= 0 && pointId < static_cast<int>(points.size()))
    {
        points[pointId] = deletedPoint;
        points[pointId].active = true;

        for (int ci : cloudsWithPoint)
        {
            if (ci >= 0 && ci < static_cast<int>(clouds.size()))
            {
                clouds[ci].pointIds.push_back(pointId);
            }
        }
    }
}

void DeleteCommand::redo()
{
    if (!executed)
        return;

    auto& points = scene->getPointsMutable();
    auto& clouds = scene->getCloudsMutable();

    if (pointId >= 0 && pointId < static_cast<int>(points.size()))
    {
        points[pointId].active = false;

        for (int ci : cloudsWithPoint)
        {
            if (ci >= 0 && ci < static_cast<int>(clouds.size()))
            {
                auto& vec = clouds[ci].pointIds;
                vec.erase(remove(vec.begin(), vec.end(), pointId), vec.end());
            }
        }
    }
}

