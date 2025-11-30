#pragma once
#include "node.h"
#include <vector>
#include <memory>

using namespace std;

struct Cloud
{
    int id;
    vector<int> pointIds;
    size_t textureIndex;
    
    Cloud(int id_) : id(id_), textureIndex(0) {}
    
    static Cloud fromCloudNode(const CloudNode& cloudNode, const vector<Point>& points);
    
    static vector<int> getPointIdsFromNode(const CloudNode& cloudNode, const vector<Point>& points);
};
