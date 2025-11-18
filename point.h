#pragma once
#include <string>

struct Point
{
    int id;
    int x, y;
    std::string texture;
    bool active;

    Point(int id_, int x_, int y_)
        : id(id_), x(x_), y(y_), texture(), active(true) {}
};