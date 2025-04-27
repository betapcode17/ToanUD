#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Point
{
    int x, y;

    // For sorting points
    bool operator<(const Point &p) const
    {
        return x < p.x || (x == p.x && y < p.y);
    }

    // For comparing points
    bool operator==(const Point &p) const
    {
        return x == p.x && y == p.y;
    }
};

// To find orientation of triplet (p, q, r)
int orientation(Point p, Point q, Point r)
{
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0)
        return 0;             // collinear
    return (val > 0) ? 1 : 2; // 1 for clockwise, 2 for counter-clockwise
}

vector<Point> convexHullJarvisMarch(vector<Point> points)
{
    int n = points.size();
    if (n < 3)
        return {}; // Convex hull not possible

    vector<Point> hull;

    // Find the leftmost point
    int leftmost = 0;
    for (int i = 1; i < n; i++)
    {
        if (points[i].x < points[leftmost].x ||
            (points[i].x == points[leftmost].x && points[i].y < points[leftmost].y))
        {
            leftmost = i;
        }
    }

    int p = leftmost, q;
    do
    {
        hull.push_back(points[p]);

        q = (p + 1) % n;
        for (int i = 0; i < n; i++)
        {
            // If i is more counter-clockwise than current q, then update q
            if (orientation(points[p], points[i], points[q]) == 2)
            {
                q = i;
            }
        }

        p = q;
    } while (p != leftmost); // Until we wrap around to the first point

    return hull;
}

int main()
{
    vector<Point> points = {
        {0, 3}, {2, 2}, {1, 1}, {2, 1}, {3, 0}, {0, 0}, {3, 3}, {1, 2}};

    vector<Point> hull = convexHullJarvisMarch(points);

    cout << "Convex Hull Points (Jarvis March):\n";
    for (auto p : hull)
    {
        cout << "(" << p.x << ", " << p.y << ")\n";
    }

    return 0;
}