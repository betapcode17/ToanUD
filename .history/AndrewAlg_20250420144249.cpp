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
// Tích có hướng OA và OB
// Nếu tích có hướng > 0 thì OAB là tam giác có hướng ngược chiều kim đồng hồ
// Nếu tích có hướng < 0 thì OAB là tam giác có hướng cùng chiều kim đồng hồ
// Nếu tích có hướng = 0 thì OAB là 3 điểm thẳng hàng
int cross_prouduct(Point O, Point A, Point B)
{
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}
// Trả về danh sách các đỉnh của đa giác lồi
//  Đầu vào là danh sách các điểm không trùng nhau
//  Đầu ra là danh sách các đỉnh của đa giác lồi theo thứ tự ngược chiều kim đồng hồ
vector<Point> convex_hull(vector<Point> points)
{
    int n = points.size();
    if (n < 3)
        return {};

    vector<Point> ans(2 * n);
    sort(points.begin(), points.end());
    vector<Point> hull;
    for (int i = 0; i < n; i++)
    {
        while (hull.size() >= 2 && cross_prouduct(hull[hull.size() - 2], hull[hull.size() - 1], points[i]) <= 0)
            hull.pop_back();
        hull.push_back(points[i]);
    }
    // Tạo nửa trên của đa giác lồi
    int lower_size = hull.size() + 1;
    for (int i = n - 1; i >= 0; i--)
    {
        while (hull.size() > lower_size && cross_prouduct(hull[hull.size() - 2], hull[hull.size() - 1], points[i]) <= 0)
            hull.pop_back();
        hull.push_back(points[i]);
    }
    hull.pop_back(); // Xóa điểm đầu tiên để không bị trùng với điểm cuối cùng
    return hull;
}