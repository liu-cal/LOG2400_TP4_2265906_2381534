#include "grid.h"
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

Grid::Grid(int width, int height) : width(width), height(height)
{
    grid.resize(height);
    for (int i = 0; i < height; ++i)
    {
        grid[i].resize(width, " ");
    }
}

void Grid::setCell(int x, int y, const string& content)
{
    if (isValidPosition(x, y))
    {
        grid[y][x] = content;
    }
}

void Grid::setCell(int x, int y, char content)
{
    setCell(x, y, string(1, content));
}

string Grid::getCell(int x, int y) const
{
    if (isValidPosition(x, y))
    {
        return grid[y][x];
    }
    return " ";
}

void Grid::traceLine(int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int steps = max(abs(dx), abs(dy));
    
    for (int i = 1; i < steps; ++i)
    {
        double t = (double)i / steps;
        int x = round(x0 + t * dx);
        int y = round(y0 + t * dy);
        
        if (isValidPosition(x, y) && getCell(x, y) == " ")
        {
            if (dx == 0)
                setCell(x, y, '|');
            else if (dy == 0)
                setCell(x, y, '-');
            else
            {
                if ((dx > 0 && dy > 0) || (dx < 0 && dy < 0))
                    setCell(x, y, "/");
                else
                    setCell(x, y, "\\");
            }
        }
    }
}

void Grid::traceSurfaces(const vector<vector<int>>& surfaces, const vector<Point>& points)
{
    for (const auto &poly : surfaces)
    {
        if (poly.size() < 2)
            continue;

        for (size_t i = 0; i < poly.size(); ++i)
        {
            int idA = poly[i];
            int idB = poly[(i + 1) % poly.size()];

            const Point &a = points[idA];
            const Point &b = points[idB];

            traceLine(a.x, a.y, b.x, b.y);
        }
    }
}

void Grid::print() const
{
    for (int y = height - 1; y >= 0; --y)
    {
        for (int x = 0; x < width; ++x)
            cout << grid[y][x];
        cout << '\n';
    }
}

bool Grid::isValidPosition(int x, int y) const
{
    return x >= 0 && x < width && y >= 0 && y < height;
}

