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
    int k = 0; // Declare and initialize k
    for (int i = 0; i < n; ++i)
    {

        while (
            k >= 2 && cross_prouduct(ans[k - 2], ans[k - 1], points[i]) <= 0)
            k--;
        ans[k++] = points[i];
    }
    // Build upper hull
    for (size_t i = n - 1, t = k + 1; i > 0; --i)
    {

        // If the point at K-1 position is not a part
        // of hull as vector from ans[k-2] to ans[k-1]
        // and ans[k-2] to A[i] has a clockwise turn
        while (k >= t && cross_product(ans[k - 2], ans[k - 1],
                                       A[i - 1]) <= 0)
            k--;
        ans[k++] = A[i - 1];
    }
}