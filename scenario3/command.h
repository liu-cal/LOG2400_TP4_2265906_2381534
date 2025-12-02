#pragma once
#include <memory>
#include <vector>
#include "point.h"

class Scene;

class ICommand
{
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual void redo() = 0;
};

class CommandManager
{
public:
    void executeCommand(unique_ptr<ICommand> command);
    void undo();
    void redo();

private:
    vector<unique_ptr<ICommand>> undoStack;
    vector<unique_ptr<ICommand>> redoStack;
};

class MoveCommand : public ICommand
{
public:
    MoveCommand(Scene* scene, int pointId, int newX, int newY);
    void execute() override;
    void undo() override;
    void redo() override;

private:
    Scene* scene;
    int pointId;
    int oldX, oldY;
    int newX, newY;
    bool executed;
};

class DeleteCommand : public ICommand
{
public:
    DeleteCommand(Scene* scene, int pointId);
    void execute() override;
    void undo() override;
    void redo() override;

private:
    Scene* scene;
    int pointId;
    Point deletedPoint;
    vector<int> cloudsWithPoint;
    bool executed;
};

