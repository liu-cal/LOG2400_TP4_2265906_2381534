#pragma once
#include <vector>
#include <cstddef>

struct Cloud
{
    int id;
    std::vector<int> pointIds;
    size_t textureIndex;
    Cloud(int id_) : id(id_), textureIndex(0) {}
};
