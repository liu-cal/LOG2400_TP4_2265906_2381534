#include "display.h"
#include "textureManager.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

void tracerLigne(std::vector<std::vector<char>> &grille, int x0, int y0, int x1, int y1)
{
    int lignes = std::abs(y1 - y0);
    if (lignes == 0)
    {
        lignes = std::abs(x1 - x0);
        for (int i = 0; i <= lignes; ++i)
        {
            int x = (x0 < x1) ? x0 + i : x0 - i;
            if (y1 >= 0 && y1 < HAUTEUR && x >= 0 && x < LARGEUR)
                grille[y1][x] = '/';
        }
    }
    else
    {
        for (int i = 0; i <= lignes; ++i)
        {
            double t = (double)i / lignes;
            int x = (int)std::round(x0 + t * (x1 - x0));
            int y = (int)std::round(y0 + t * (y1 - y0));
            if (x >= 0 && x < LARGEUR && y >= 0 && y < HAUTEUR)
                grille[y][x] = '/';
        }
    }
}

void DisplayTextures::draw(const std::vector<Point> &points, const std::vector<Cloud> &clouds, const TextureManager &tm)
{
    std::vector<std::vector<char>> grille(HAUTEUR, std::vector<char>(LARGEUR, ' '));
    std::vector<char> mapTexture(points.size(), '.');
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
                mapTexture[pid] = t;
        }
    }

    for (const auto &p : points)
    {
        if (!p.active)
            continue;
        if (p.x >= 0 && p.x < LARGEUR && p.y >= 0 && p.y < HAUTEUR)
            grille[p.y][p.x] = (mapTexture[p.id] == '.' ? '.' : mapTexture[p.id]);
    }

    for (int y = HAUTEUR - 1; y >= 0; --y)
    {
        for (int x = 0; x < LARGEUR; ++x)
            std::cout << grille[y][x];
        std::cout << '\n';
    }
}

void DisplayIDs::draw(const std::vector<Point> &points, const std::vector<Cloud> &clouds, const TextureManager &tm)
{
    std::vector<std::vector<char>> grille(HAUTEUR, std::vector<char>(LARGEUR, ' '));
    for (const auto &p : points)
    {
        if (!p.active)
            continue;
        char ch = (p.id < 10 ? char('0' + p.id) : '*');
        if (p.x >= 0 && p.x < LARGEUR && p.y >= 0 && p.y < HAUTEUR)
            grille[p.y][p.x] = ch;
    }
    for (int y = HAUTEUR - 1; y >= 0; --y)
    {
        for (int x = 0; x < LARGEUR; ++x)
            std::cout << grille[y][x];
        std::cout << '\n';
    }
}
