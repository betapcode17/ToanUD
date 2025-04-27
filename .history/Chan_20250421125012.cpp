#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

struct Point
{
    double x, y;
};

bool rightTurn(const Point &p1, const Point &p2, const Point &p3)
{
    return (p3.y - p1.y) * (p2.x - p1.x) < (p2.y - p1.y) * (p3.x - p1.x);
}
bool orient(const Point &p1, const Point &p2, const Point &p3)
{
    return (p3.y - p1.y) * (p2.x - p1.x) >= (p2.y - p1.y) * (p3.x - p1.x);
}
std::vector<Point> grahamScan(std::vector<Point> P)
{
    std::sort(P.begin(), P.end(), [](Point a, Point b)
              { return a.y < b.y || (a.y == b.y && a.x < b.x); });

    if (P.size() < 2)
        return P;

    std::vector<Point> upper{P[0], P[1]};
    for (size_t i = 2; i < P.size(); i++)
    {
        upper.push_back(P[i]);
        while (upper.size() > 2 && !rightTurn(upper[upper.size() - 1], upper[upper.size() - 2], upper[upper.size() - 3]))
        {
            upper.erase(upper.end() - 2);
        }
    }

    std::vector<Point> lower{P.back(), P[P.size() - 2]};
    for (int i = P.size() - 3; i >= 0; i--)
    {
        lower.push_back(P[i]);
        while (lower.size() > 2 && !rightTurn(lower[lower.size() - 1], lower[lower.size() - 2], lower[lower.size() - 3]))
        {
            lower.erase(lower.end() - 2);
        }
    }

    lower.erase(lower.begin());
    lower.pop_back();

    upper.insert(upper.end(), lower.begin(), lower.end());
    return upper;
}

std::vector<Point> giftWrapping(const std::vector<Point> &G, int m)
{
    int n = G.size();
    std::vector<Point> hull;
    Point start = *std::min_element(G.begin(), G.end(), [](Point a, Point b)
                                    { return a.x < b.x; });

    Point current = start;
    int t = 0;
    do
    {
        hull.push_back(current);
        Point next = G[0];
        for (const auto &p : G)
        {
            if ((next.x == current.x && next.y == current.y) || !orient(p, current, next))
            {
                next = p;
            }
        }
        current = next;
        t++;
        if (t > m)
        {
            std::cout << "More than m points on hull. Increase m.\n";
            return {};
        }
    } while (!(current.x == start.x && current.y == start.y));

    return hull;
}
