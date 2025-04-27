#include <iostream>
#include <vector>
#include <algorithm>
#include <cfloat>
#include <cmath>

using namespace std;
struct Point
{
    int x, y;
    bool operator<(Point p)
    {
        return x < p.x || (x == p.x && y < p.y);
    }
};
int cross_prouduct(Point O, Point A, Point B)
{
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}
vector<Point> convex_hull(vector<Point> points)
{
    int n = points.size();
    if (n < 3)
        return {};

    vector<Point> ans(2 * n);
    sort(points.begin(), points.end());
    vector<Point> hull;
    int k = 0;
    for (int i = 0; i < n; ++i)
    {

        while (
            k >= 2 && cross_prouduct(ans[k - 2], ans[k - 1], points[i]) <= 0)
            k--;
        ans[k++] = points[i];
    }
    for (size_t i = n - 1, t = k + 1; i > 0; --i)
    {
        while (k >= t && cross_prouduct(ans[k - 2], ans[k - 1],
                                        points[i - 1]) <= 0)
            k--;
        ans[k++] = points[i - 1];
    }
    ans.resize(k - 1);
    return ans;
}
// Tính diện tích bao lồi đã tìm được
double area(vector<Point> points)
{
    double area = 0.0;
    int n = points.size();
    for (int i = 0; i < n; i++)
    {
        area += (points[i].x * points[(i + 1) % n].y) -
                (points[i].y * points[(i + 1) % n].x);
    }
    return abs(area) / 2.0;
}
// Tìm khoảng cách ngắn nhất các cạnh nằm trên bao lồi
double min_distance(vector<Point> points)
{
    double min_dist = DBL_MAX;
    int n = points.size();
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            double dist = sqrt(pow(points[i].x - points[j].x, 2) +
                               pow(points[i].y - points[j].y, 2));
            min_dist = min(min_dist, dist);
        }
    }
    return min_dist;
}
// liệt kê các điểm nằm trong bao lồi
vector<Point> points_inside_hull(vector<Point> points, vector<Point> hull)
{
    vector<Point> inside_points;
    for (Point p : points)
    {
        bool inside = true;
        for (int i = 0; i < hull.size(); i++)
        {
            if (cross_prouduct(hull[i], hull[(i + 1) % hull.size()], p) < 0)
            {
                inside = false;
                break;
            }
        }
        if (inside)
            inside_points.push_back(p);
    }
    return inside_points;
}
int main()
{
    vector<Point> points;
    points.push_back({0, 3});
    points.push_back({2, 2});
    points.push_back({1, 1});
    points.push_back({2, 1});
    points.push_back({3, 0});
    points.push_back({0, 0});
    points.push_back({3, 3});
    vector<Point> ans = convex_hull(points);
    for (int i = 0; i < ans.size(); i++)
        cout << "(" << ans[i].x << ", " << ans[i].y << ")"
             << endl;

    return 0;
}