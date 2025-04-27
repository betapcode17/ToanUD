#include <stdio.h>
#include <stdlib.h>

#define MAXN 1000

struct Point
{
    int x, y;
};
Point mid;
int quad(Point p)
{
    if (p.x >= 0 && p.y >= 0)
        return 1;
    if (p.x <= 0 && p.y >= 0)
        return 2;
    if (p.x <= 0 && p.y <= 0)
        return 3;
    return 4;
}
int orientation(Point a, Point b, Point c)
{
    int res = (b.y - a.y) * (c.x - b.x) - (c.y - b.y) * (b.x - a.x);
    if (res == 0)
        return 0;
    return (res > 0) ? 1 : -1;
}
int compare(const void *a, const void *b)
{
    Point p1 = *(Point *)a;
    Point p2 = *(Point *)b;
    Point p = {p1.x - mid.x, p1.y - mid.y};
    Point q = {p2.x - mid.x, p2.y - mid.y};
    int one = quad(p);
    int two = quad(q);
    if (one != two)
        return one - two;
    return (p.y * q.x - q.y * p.x);
}
void bruteHull(Point *a, int n, Point *ret, int *ret_size)
{
    int i, j, k;
    *ret_size = 0;

    for (i = 0; i < n; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            int x1 = a[i].x, x2 = a[j].x;
            int y1 = a[i].y, y2 = a[j].y;

            int a1 = y1 - y2;
            int b1 = x2 - x1;
            int c1 = x1 * y2 - y1 * x2;

            int pos = 0, neg = 0;
            for (k = 0; k < n; k++)
            {
                int val = a1 * a[k].x + b1 * a[k].y + c1;
                if (val <= 0)
                    neg++;
                if (val >= 0)
                    pos++;
            }
            if (pos == n || neg == n)
            {
                int exist = 0;
                for (k = 0; k < *ret_size; k++)
                    if (ret[k].x == a[i].x && ret[k].y == a[i].y)
                        exist = 1;
                if (!exist)
                    ret[(*ret_size)++] = a[i];

                exist = 0;
                for (k = 0; k < *ret_size; k++)
                    if (ret[k].x == a[j].x && ret[k].y == a[j].y)
                        exist = 1;
                if (!exist)
                    ret[(*ret_size)++] = a[j];
            }
        }
    }

    mid.x = 0;
    mid.y = 0;
    for (i = 0; i < *ret_size; i++)
    {
        mid.x += ret[i].x;
        mid.y += ret[i].y;
    }
    mid.x /= *ret_size;
    mid.y /= *ret_size;

    qsort(ret, *ret_size, sizeof(Point), compare);
}
void merger(Point *a, int n1, Point *b, int n2, Point *ret, int *ret_size)
{
    int ia = 0, ib = 0, i;
    for (i = 1; i < n1; i++)
        if (a[i].x > a[ia].x)
            ia = i;
    for (i = 1; i < n2; i++)
        if (b[i].x < b[ib].x)
            ib = i;

    int inda = ia, indb = ib;
    int done = 0;
    while (!done)
    {
        done = 1;
        while (orientation(b[indb], a[inda], a[(inda + 1) % n1]) >= 0)
            inda = (inda + 1) % n1;
        while (orientation(a[inda], b[indb], b[(n2 + indb - 1) % n2]) <= 0)
        {
            indb = (n2 + indb - 1) % n2;
            done = 0;
        }
    }
    int uppera = inda, upperb = indb;

    inda = ia;
    indb = ib;
    done = 0;
    while (!done)
    {
        done = 1;
        while (orientation(a[inda], b[indb], b[(indb + 1) % n2]) >= 0)
            indb = (indb + 1) % n2;
        while (orientation(b[indb], a[inda], a[(n1 + inda - 1) % n1]) <= 0)
        {
            inda = (n1 + inda - 1) % n1;
            done = 0;
        }
    }
    int lowera = inda, lowerb = indb;

    *ret_size = 0;
    int ind = uppera;
    ret[(*ret_size)++] = a[uppera];
    while (ind != lowera)
    {
        ind = (ind + 1) % n1;
        ret[(*ret_size)++] = a[ind];
    }
    ind = lowerb;
    ret[(*ret_size)++] = b[lowerb];
    while (ind != upperb)
    {
        ind = (ind + 1) % n2;
        ret[(*ret_size)++] = b[ind];
    }
}
void divide(Point *a, int n, Point *ans, int *ans_size)
{
    if (n <= 5)
    {
        bruteHull(a, n, ans, ans_size);
        return;
    }

    Point left[MAXN], right[MAXN];
    int nl = 0, nr = 0;
    int i;
    for (i = 0; i < n / 2; i++)
        left[nl++] = a[i];
    for (i = n / 2; i < n; i++)
        right[nr++] = a[i];

    Point left_hull[MAXN], right_hull[MAXN];
    int left_size, right_size;

    divide(left, nl, left_hull, &left_size);
    divide(right, nr, right_hull, &right_size);

    merger(left_hull, left_size, right_hull, right_size, ans, ans_size);
}
int cmp_x(const void *a, const void *b)
{
    Point p1 = *(Point *)a;
    Point p2 = *(Point *)b;
    if (p1.x != p2.x)
        return p1.x - p2.x;
    return p1.y - p2.y;
}

int main()
{
    Point a[MAXN] = {
        {0, 0}, {1, -4}, {-1, -5}, {-5, -3}, {-3, -1}, {-1, -3}, {-2, -2}, {-1, -1}, {-2, -1}, {-1, 1}};
    int n = 10;

    qsort(a, n, sizeof(Point), cmp_x);

    Point ans[MAXN];
    int ans_size;

    divide(a, n, ans, &ans_size);

    printf("convex hull:\n");
    for (int i = 0; i < ans_size; i++)
        printf("%d %d\n", ans[i].x, ans[i].y);

    return 0;
}
