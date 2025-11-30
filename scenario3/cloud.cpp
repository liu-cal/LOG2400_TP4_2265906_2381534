#include "cloud.h"
#include "node.h"
#include "point.h"

using namespace std;

Cloud Cloud::fromCloudNode(const CloudNode& cloudNode, const vector<Point>& points)
{
    Cloud cloud(cloudNode.getId());
    cloud.textureIndex = cloudNode.getTextureIndex();
    cloud.pointIds = getPointIdsFromNode(cloudNode, points);
    return cloud;
}

vector<int> Cloud::getPointIdsFromNode(const CloudNode& cloudNode, const vector<Point>& points)
{
    return cloudNode.getAllPointIds(points);
}

