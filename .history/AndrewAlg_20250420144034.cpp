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
