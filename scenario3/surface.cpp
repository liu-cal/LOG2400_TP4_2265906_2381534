#include "surface.h"
#include <algorithm>
#include <cmath>
#include <limits>

vector<int> SurfaceByIDOrder::build(const Cloud &cloud, const vector<Point> &allPoints)
{
    vector<int> ids = cloud.pointIds;
    sort(ids.begin(), ids.end());
    return ids;
}

vector<int> SurfaceByNearest::build(const Cloud &cloud, const vector<Point> &allPoints)
{
    vector<int> remaining = cloud.pointIds;
    vector<int> result;
    if (remaining.empty())
        return result;

    int startIdx = *min_element(remaining.begin(), remaining.end());
    result.push_back(startIdx);
    remaining.erase(find(remaining.begin(), remaining.end(), startIdx));
    while (!remaining.empty())
    {
        int last = result.back();
        const Point &pl = allPoints[last];
        double bestDist = numeric_limits<double>::infinity();
        int bestId = remaining.front();
        for (int cand : remaining)
        {
            const Point &pc = allPoints[cand];
            double dx = pl.x - pc.x, dy = pl.y - pc.y;
            double d = sqrt(dx * dx + dy * dy);
            if (d < bestDist)
            {
                bestDist = d;
                bestId = cand;
            }
        }
        result.push_back(bestId);
        remaining.erase(find(remaining.begin(), remaining.end(), bestId));
    }
    return result;
}
