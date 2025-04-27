#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
struct Point
{
    int x, y;
    bool operator<(Point p)
    {
        return x < p.x || (x == p.x && y < p.y);
    }
};