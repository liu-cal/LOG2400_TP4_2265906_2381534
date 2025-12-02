#pragma once
#include <vector>
#include <algorithm>

class TextureManager
{
    vector<char> textures;

public:
    TextureManager()
    {
        textures = {'o', '#', '$'};
    }

    char get(size_t idx) const
    {
        if (textures.empty())
            return '.';
        return textures[idx % textures.size()];
    }

    size_t count() const { return textures.size(); }
};
