#include "surface.h"
#include <algorithm>
#include <cmath>
#include <limits>

std::vector<int> SurfaceByIDOrder::build(const Cloud &cloud, const std::vector<Point> &allPoints)
{
    std::vector<int> ids = cloud.pointIds;
    std::sort(ids.begin(), ids.end());
    return ids;
}

std::vector<int> SurfaceByNearest::build(const Cloud &cloud, const std::vector<Point> &allPoints)
{
    std::vector<int> remaining = cloud.pointIds;
    std::vector<int> result;
    if (remaining.empty())
        return result;

    int startIdx = *std::min_element(remaining.begin(), remaining.end());
    result.push_back(startIdx);
    remaining.erase(std::find(remaining.begin(), remaining.end(), startIdx));
    while (!remaining.empty())
    {
        int last = result.back();
        const Point &pl = allPoints[last];
        double bestDist = std::numeric_limits<double>::infinity();
        int bestId = remaining.front();
        for (int cand : remaining)
        {
            const Point &pc = allPoints[cand];
            double dx = pl.x - pc.x, dy = pl.y - pc.y;
            double d = std::sqrt(dx * dx + dy * dy);
            if (d < bestDist)
            {
                bestDist = d;
                bestId = cand;
            }
        }
        result.push_back(bestId);
        remaining.erase(std::find(remaining.begin(), remaining.end(), bestId));
    }
    return result;
}
