#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

void swap(Point& a, Point& b) {
    Point temp = a;
    a = b;
    b = temp;
}

double tichCoHuong(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

double dist2(Point a, Point b) {
    return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
}

bool soSanh(Point O, Point a, Point b) {
    double tich = tichCoHuong(O, a, b);
    if (fabs(tich) < 1e-9)
        return dist2(O, a) < dist2(O, b); // So sánh khoảng cách nếu thẳng hàng
    return tich > 0;
}

int ccw(const Point& a, const Point& b, const Point& c) {
    int area = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    if (area > 0) return 1;   // Ngược chiều kim đồng hồ
    if (area < 0) return -1;  // Cùng chiều kim đồng hồ
    return 0;                 // Thẳng hàng
}

int GrahamScan(Point Points[], int n, Point out[]) {
    if (n < 3) return 0; // Không đủ điểm để tạo bờ lồi

    int min_idx = 0;
    for(int i = 1; i < n; i++) {
        if (Points[i].y < Points[min_idx].y || ((Points[i].y == Points[min_idx].y) && (Points[i].x < Points[min_idx].x))) {
            min_idx = i;
        }
    }
    swap(Points[0], Points[min_idx]);
    Point O = Points[0]; // Bây giờ O chính là n[0]

    for(int i = 1; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            if (!soSanh(O, Points[i], Points[j])) {
                swap(Points[i], Points[j]);
            }
        }
    }

    int GrahamScanSize = 2;
    out[0] = Points[0];
    out[1] = Points[1];
    for(int i = 2; i < n; i++) {
        while(GrahamScanSize > 1 && ccw(out[GrahamScanSize - 2], out[GrahamScanSize - 1], Points[i]) <= 0) {
            GrahamScanSize--;
        }
        out[GrahamScanSize++] = Points[i];
    }
    return GrahamScanSize;
}

void Chan(Point Points[], int n) {
    const int MAXN = 1000;
    const int MAXK = 100;
    Point hulls[MAXN / MAXK + 2][MAXK + 2]; // các bao lồi nhỏ
    int hullSizes[MAXN / MAXK + 2];

    int m, k;
    for (k = 1; k <= n; k *= 2) {
        m = (n + k - 1) / k; // số nhóm
        // Tìm bao lồi cho từng nhóm nhỏ
        for (int i = 0; i < m; ++i) {
            int left = i * k;
            int right = (left + k < n) ? (left + k) : n;
            hullSizes[i] = 0;
            if (right - left < 3) {
                for (int j = left; j < right; ++j)
                    hulls[i][hullSizes[i]++] = Points[j];
                continue;
            }
            Point temp[MAXK + 2], out[MAXK + 2];
            for (int j = left; j < right; ++j)
                temp[j - left] = Points[j];
            int sz = GrahamScan(temp, right - left, out);
            for (int j = 0; j < sz; ++j)
                hulls[i][j] = out[j];
            hullSizes[i] = sz;
        }

        // Tìm điểm trái nhất
        int leftMost = 0;
        for (int i = 1; i < n; ++i)
            if (Points[i].x < Points[leftMost].x || (Points[i].x == Points[leftMost].x && Points[i].y < Points[leftMost].y))
                leftMost = i;

        Point hull[MAXN + 2];
        int hullSize = 0;
        hull[hullSize++] = Points[leftMost];
        Point cur = Points[leftMost];

        for (int t = 0; t < k * 2; ++t) { // tối đa k*2 bước
            Point next;
            bool found = false;
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < hullSizes[i]; ++j) {
                    Point p = hulls[i][j];
                    if (p.x == cur.x && p.y == cur.y) continue;
                    if (!found) {
                        next = p;
                        found = true;
                    } else {
                        int c = ccw(cur, next, p);
                        if (c < 0 || (c == 0 && dist2(cur, p) > dist2(cur, next))) {
                            next = p;
                        }
                    }
                }
            }
            if (next.x == hull[0].x && next.y == hull[0].y) break; // đóng vòng
            hull[hullSize++] = next;
            cur = next;
            if (hullSize > n) break; // không thể có nhiều hơn n điểm
        }

        if (hullSize <= k) {
            // In kết quả
            cout << "Chan's Convex Hull:\n";
            for (int i = 0; i < hullSize; ++i)
                cout << "(" << hull[i].x << ", " << hull[i].y << ")\n";
            return;
        }
    }
    cout << "Không tìm được bao lồi với Chan's algorithm!\n";
}

int main() {
    Point Points[] = {
        {0, 0}, {1, 1}, {2, 2}, {3, 3},
        {3, 0}, {0, 3}, {1, 2}, {2, 1}
    };
    int n = sizeof(Points) / sizeof(Points[0]);
    Chan(Points, n);
    return 0;
}