#include "display.h"
#include "textureManager.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

void tracerLigne(vector<vector<string>> &grille, int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int steps = max(abs(dx), abs(dy));
    for (int i = 1; i < steps; ++i)
    {
        double t = (double)i / steps;
        int x = round(x0 + t * dx);
        int y = round(y0 + t * dy);
        if (x >= 0 && x < LARGEUR && y >= 0 && y < HAUTEUR)
        {
            if (grille[y][x] == " ")
            {
                if (dx == 0)
                    grille[y][x] = '|';
                else if (dy == 0)
                    grille[y][x] = '-';
                else
                    grille[y][x] = '/';
            }
        }
    }
}

void tracerSurfaces(
    vector<vector<string>> &grille,
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

            tracerLigne(grille, a.x, a.y, b.x, b.y);
        }
    }
}

void DisplayTextures::draw(
    const vector<Point> &points,
    const vector<Cloud> &clouds,
    const vector<vector<int>> &surfaces,
    const TextureManager &tm)
{
    vector<vector<string>> grille(HAUTEUR, vector<string>(LARGEUR, " "));
    vector<string> mapTexture(points.size(), "");
    for (const auto &p : points)
    {
        if (p.id >= 0 && p.id < (int)points.size())
            mapTexture[p.id] = p.texture;
    }

    for (const auto &c : clouds)
    {
        char t = tm.get(c.textureIndex);
        for (int pid : c.pointIds)
        {
            if (pid >= 0 && pid < (int)points.size())
                mapTexture[pid] += t;
        }
    }

    for (const auto &p : points)
    {
        if (!p.active)
            continue;
        if (p.x >= 0 && p.x < LARGEUR && p.y >= 0 && p.y < HAUTEUR)
            grille[p.y][p.x] = (mapTexture[p.id] == "" ? "." : mapTexture[p.id]);
    }

    for (int y = HAUTEUR - 1; y >= 0; --y)
    {
        for (int x = 0; x < LARGEUR; ++x)
            cout << grille[y][x];
        cout << '\n';
    }
}

void DisplayIDs::draw(
    const vector<Point> &points,
    const vector<Cloud> &clouds,
    const vector<vector<int>> &surfaces,
    const TextureManager &tm)
{
    vector<vector<string>> grille(HAUTEUR, vector<string>(LARGEUR, " "));
    for (const auto &p : points)
    {
        if (!p.active)
            continue;
        char ch = (p.id < 10 ? char('0' + p.id) : '*');
        if (p.x >= 0 && p.x < LARGEUR && p.y >= 0 && p.y < HAUTEUR)
            grille[p.y][p.x] = ch;
    }

    tracerSurfaces(grille, surfaces, points);

    for (int y = HAUTEUR - 1; y >= 0; --y)
    {
        for (int x = 0; x < LARGEUR; ++x)
            cout << grille[y][x];
        cout << '\n';
    }
}
