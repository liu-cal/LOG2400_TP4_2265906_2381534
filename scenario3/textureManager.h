#pragma once
#include <vector>
#include <algorithm>

class TextureManager
{
    std::vector<char> textures;

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

    void addTexture(char c) { textures.push_back(c); }

    void removeTexture(char c)
    {
        textures.erase(std::remove(textures.begin(), textures.end(), c), textures.end());
    }
    size_t count() const { return textures.size(); }
};
