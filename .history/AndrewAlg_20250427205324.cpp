#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

typedef struct
{
    int x, y;
} Point;

// So sánh 2 điểm theo x rồi y
int compare_points(const void *a, const void *b)
{
    Point *p1 = (Point *)a;
    Point *p2 = (Point *)b;
    if (p1->x != p2->x)
        return p1->x - p2->x;
    else
        return p1->y - p2->y;
}

// Tính cross product (tích có hướng)
int cross_product(Point O, Point A, Point B)
{
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// Thuật toán Andrew tìm convex hull
int AndrewsAlgorithm(Point points[], int n, Point hull[])
{
    if (n < 3)
        return 0;

    qsort(points, n, sizeof(Point), compare_points);

    int k = 0;
    for (int i = 0; i < n; i++)
    {
        while (k >= 2 && cross_product(hull[k - 2], hull[k - 1], points[i]) <= 0)
            k--;
        hull[k++] = points[i];
    }

    for (int i = n - 2, t = k + 1; i >= 0; i--)
    {
        while (k >= t && cross_product(hull[k - 2], hull[k - 1], points[i]) <= 0)
            k--;
        hull[k++] = points[i];
    }

    k--; // loại bỏ điểm trùng đầu-cuối
    return k;
}

// Tính diện tích bao lồi
double area(Point points[], int n)
{
    double result = 0.0;
    for (int i = 0; i < n; i++)
    {
        result += (points[i].x * points[(i + 1) % n].y) -
                  (points[i].y * points[(i + 1) % n].x);
    }
    return fabs(result) / 2.0;
}

// Khoảng cách ngắn nhất giữa các điểm trên bao lồi
double min_distance(Point points[], int n)
{
    double min_dist = DBL_MAX;
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            double dist = sqrt(pow(points[i].x - points[j].x, 2) +
                               pow(points[i].y - points[j].y, 2));
            if (dist < min_dist)
                min_dist = dist;
        }
    }
    return min_dist;
}

// Kiểm tra điểm p có nằm trong bao lồi hay không
int is_inside_hull(Point p, Point hull[], int hull_size)
{
    for (int i = 0; i < hull_size; i++)
    {
        if (cross_product(hull[i], hull[(i + 1) % hull_size], p) < 0)
            return 0; // outside
    }
    return 1; // inside or on edge
}

// Lấy danh sách các điểm nằm trong hoặc trên bao lồi
int points_inside_hull(Point points[], int n, Point hull[], int hull_size, Point inside_points[])
{
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        if (is_inside_hull(points[i], hull, hull_size))
        {
            inside_points[count++] = points[i];
        }
    }
    return count;
}

// Khoảng cách ngắn nhất từ các điểm nằm trong bao lồi đến bao lồi
double min_distance_inside_hull(Point points[], int n, Point hull[], int hull_size)
{
    double min_dist = DBL_MAX;
    Point inside_points[1000]; // tối đa 1000 điểm
    int inside_count = points_inside_hull(points, n, hull, hull_size, inside_points);

    for (int i = 0; i < inside_count; i++)
    {
        for (int j = 0; j < hull_size; j++)
        {
            double dist = sqrt(pow(inside_points[i].x - hull[j].x, 2) +
                               pow(inside_points[i].y - hull[j].y, 2));
            if (dist < min_dist)
                min_dist = dist;
        }
    }
    return min_dist;
}

int main()
{
    Point points[] = {
        {0, 3}, {2, 2}, {1, 1}, {2, 1}, {3, 0}, {0, 0}, {3, 3}};
    int n = sizeof(points) / sizeof(points[0]);

    Point hull[2 * 1000];
    int hull_size = AndrewsAlgorithm(points, n, hull);

    printf("Points on convex hull:\n");
    for (int i = 0; i < hull_size; i++)
    {
        printf("(%d, %d)\n", hull[i].x, hull[i].y);
    }

    printf("Area of convex hull: %.2lf\n", area(hull, hull_size));
    printf("Minimum distance between points on convex hull: %.6lf\n", min_distance(hull, hull_size));
    printf("Minimum distance between points inside convex hull: %.6lf\n", min_distance_inside_hull(points, n, hull, hull_size));

    Point inside_points[1000];
    int inside_count = points_inside_hull(points, n, hull, hull_size, inside_points);

    printf("Points inside convex hull:\n");
    for (int i = 0; i < inside_count; i++)
    {
        printf("(%d, %d)\n", inside_points[i].x, inside_points[i].y);
    }

    printf("Total points inside convex hull: %d\n", inside_count);
    printf("Total points on convex hull: %d\n", hull_size);
    printf("Total points: %d\n", n);
    printf("Total points on convex hull and inside convex hull: %d\n", hull_size + inside_count);

    return 0;
}
