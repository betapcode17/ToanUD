#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
class Point
{
public:
    double x, y;
    Point(int x, int y) : x(x), y(y) {}
    bool operator<(const Point &p) const
    {
        return (x < p.x) || (x == p.x && y < p.y);
    }
};
double cross(const Point &O, const Point &A, const Point &B)
{
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}
vector<Point> convexHull(vector<Point> P)
{
    int n = P.size();
    if (n <= 3)
        return P;
    sort(P.begin(), P.end());
    vector<Point> hull;
    hull.reserve(2 * n);
    for (int i = 0; i < n; ++i)
    {
        while (hull.size() >= 2 &&
               cross(hull[hull.size() - 2], hull.back(), P[i]) <= 0)
        {
            hull.pop_back();
        }
        hull.push_back(P[i]);
    }
    int lower_hull_size = hull.size();
    for (int i = n - 2; i >= 0; --i)
    {
        while (hull.size() > lower_hull_size &&
               cross(hull[hull.size() - 2], hull.back(), P[i]) <= 0)
        {
            hull.pop_back();
        }
        hull.push_back(P[i]);
    }
    hull.pop_back();

    return hull;
}