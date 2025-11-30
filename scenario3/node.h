#pragma once
#include <vector>
#include <memory>

using namespace std;

class Point;
class TextureManager;

class Node
{
public:
    virtual ~Node() = default;
    
    virtual int getId() const = 0;
    
    virtual vector<int> getAllPointIds(const vector<Point>& points) const = 0;
    
    virtual bool containsPointId(int pointId, const vector<Point>& points) const = 0;
    
    virtual void applyTexture(char textureChar, vector<Point>& points) const = 0;
    
    virtual bool isCloud() const = 0;
    virtual bool isPoint() const = 0;
};

class PointNode : public Node
{
public:
    PointNode(int pointId);
    
    int getId() const override;
    vector<int> getAllPointIds(const vector<Point>& points) const override;
    bool containsPointId(int pointId, const vector<Point>& points) const override;
    void applyTexture(char textureChar, vector<Point>& points) const override;
    bool isCloud() const override { return false; }
    bool isPoint() const override { return true; }

private:
    int pointId;
};

class CloudNode : public Node
{
public:
    CloudNode(int cloudId);
    
    int getId() const override;
    vector<int> getAllPointIds(const vector<Point>& points) const override;
    bool containsPointId(int pointId, const vector<Point>& points) const override;
    void applyTexture(char textureChar, vector<Point>& points) const override;
    bool isCloud() const override { return true; }
    bool isPoint() const override { return false; }
    
    void addChild(unique_ptr<Node> child);
    void removeChild(int nodeId);
    const vector<unique_ptr<Node>>& getChildren() const { return children; }
    size_t getTextureIndex() const { return textureIndex; }
    void setTextureIndex(size_t index) { textureIndex = index; }
    
    unique_ptr<CloudNode> clone() const;

private:
    int cloudId;
    vector<unique_ptr<Node>> children;
    size_t textureIndex;
};

