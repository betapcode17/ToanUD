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
