#include "node.h"
#include "point.h"
#include <algorithm>

using namespace std;

PointNode::PointNode(int pointId) : pointId(pointId)
{
}

int PointNode::getId() const
{
    return pointId;
}

vector<int> PointNode::getAllPointIds(const vector<Point>& points) const
{
    vector<int> result;
    if (pointId >= 0 && pointId < static_cast<int>(points.size()) && points[pointId].active)
    {
        result.push_back(pointId);
    }
    return result;
}

bool PointNode::containsPointId(int pointId, const vector<Point>& points) const
{
    return this->pointId == pointId;
}

void PointNode::applyTexture(char textureChar, vector<Point>& points) const
{
    if (pointId >= 0 && pointId < static_cast<int>(points.size()) && points[pointId].active)
    {
        points[pointId].texture += textureChar;
    }
}

CloudNode::CloudNode(int cloudId) : cloudId(cloudId), textureIndex(0)
{
}

int CloudNode::getId() const
{
    return cloudId;
}

vector<int> CloudNode::getAllPointIds(const vector<Point>& points) const
{
    vector<int> result;
    
    for (const auto& child : children)
    {
        auto childPoints = child->getAllPointIds(points);
        result.insert(result.end(), childPoints.begin(), childPoints.end());
    }
    
    sort(result.begin(), result.end());
    result.erase(unique(result.begin(), result.end()), result.end());
    
    return result;
}

bool CloudNode::containsPointId(int pointId, const vector<Point>& points) const
{
    for (const auto& child : children)
    {
        if (child->containsPointId(pointId, points))
        {
            return true;
        }
    }
    return false;
}

void CloudNode::applyTexture(char textureChar, vector<Point>& points) const
{
    for (const auto& child : children)
    {
        child->applyTexture(textureChar, points);
    }
}

void CloudNode::addChild(unique_ptr<Node> child)
{
    if (child)
    {
        children.push_back(move(child));
    }
}

void CloudNode::removeChild(int nodeId)
{
    children.erase(
        remove_if(children.begin(), children.end(),
            [nodeId](const unique_ptr<Node>& node) {
                return node->getId() == nodeId;
            }),
        children.end()
    );
}

unique_ptr<CloudNode> CloudNode::clone() const
{
    auto cloned = make_unique<CloudNode>(cloudId);
    cloned->setTextureIndex(textureIndex);
    
    for (const auto& child : children)
    {
        if (child->isPoint())
        {
            cloned->addChild(make_unique<PointNode>(child->getId()));
        }
        else if (child->isCloud())
        {
            const CloudNode* childCloud = static_cast<const CloudNode*>(child.get());
            cloned->addChild(childCloud->clone());
        }
    }
    
    return cloned;
}

