#pragma once
#include "point.h"
#include <vector>
#include <string>

using namespace std;

const int WIDTH = 30;
const int HEIGHT = 20;

class Grid
{
public:
    Grid(int width = WIDTH, int height = HEIGHT);
    
    void setCell(int x, int y, const string& content);
    void setCell(int x, int y, char content);
    
    string getCell(int x, int y) const;
    
    void traceLine(int x0, int y0, int x1, int y1);
    void traceSurfaces(const vector<vector<int>>& surfaces, const vector<Point>& points);
    
    void print() const;

private:
    vector<vector<string>> grid;
    int width;
    int height;
    
    bool isValidPosition(int x, int y) const;
};

