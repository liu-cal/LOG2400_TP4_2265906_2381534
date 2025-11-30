#include "display.h"
#include "textureManager.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

void traceLine(vector<vector<string>> &grid, int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int steps = max(abs(dx), abs(dy));
    for (int i = 1; i < steps; ++i)
    {
        double t = (double)i / steps;
        int x = round(x0 + t * dx);
        int y = round(y0 + t * dy);
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        {
            if (grid[y][x] == " ")
            {
                if (dx == 0)
                    grid[y][x] = '|';
                else if (dy == 0)
                    grid[y][x] = '-';
                else
                {
                    if ((dx > 0 && dy > 0) || (dx < 0 && dy < 0))
                        grid[y][x] = "/";
                    else
                        grid[y][x] = "\\";
                }
            }
        }
    }
}

void traceSurfaces(
    vector<vector<string>> &grid,
    const vector<vector<int>> &surfaces,
    const vector<Point> &points)
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

            traceLine(grid, a.x, a.y, b.x, b.y);
        }
    }
}

void DisplayTextures::draw(
    const vector<Point> &points,
    const vector<Cloud> &clouds,
    const vector<vector<int>> &surfaces,
    const TextureManager &tm)
{
    vector<vector<string>> grid(HEIGHT, vector<string>(WIDTH, " "));
    vector<string> mapTexture(points.size(), "");
    for (const auto &p : points)
    {
        if (p.id >= 0 && p.id < (int)points.size())
            mapTexture[p.id] = p.texture;
    }

    for (const auto &p : points)
    {
        if (!p.active)
            continue;
        if (p.x >= 0 && p.x < WIDTH && p.y >= 0 && p.y < HEIGHT)
            grid[p.y][p.x] = (mapTexture[p.id] == "" ? "." : mapTexture[p.id]);
    }

    for (int y = HEIGHT - 1; y >= 0; --y)
    {
        for (int x = 0; x < WIDTH; ++x)
            cout << grid[y][x];
        cout << '\n';
    }
}

void DisplayIDs::draw(
    const vector<Point> &points,
    const vector<Cloud> &clouds,
    const vector<vector<int>> &surfaces,
    const TextureManager &tm)
{
    vector<vector<string>> grid(HEIGHT, vector<string>(WIDTH, " "));
    for (const auto &p : points)
    {
        if (!p.active)
            continue;
        char ch = (p.id < 10 ? char('0' + p.id) : '*');
        if (p.x >= 0 && p.x < WIDTH && p.y >= 0 && p.y < HEIGHT)
            grid[p.y][p.x] = ch;
    }

    traceSurfaces(grid, surfaces, points);

    for (int y = HEIGHT - 1; y >= 0; --y)
    {
        for (int x = 0; x < WIDTH; ++x)
            cout << grid[y][x];
        cout << '\n';
    }
}
