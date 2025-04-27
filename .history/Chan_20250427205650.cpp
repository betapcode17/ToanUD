#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RIGHT_TURN -1
#define LEFT_TURN 1
#define COLLINEAR 0

struct point
{
    int x;
    int y;
};
point p0;
int dist(point p1, point p2)
{
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

int orientation(point p, point q, point r)
{
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0)
        return 0;
    return (val > 0) ? -1 : 1;
}

int compare(const void *vp1, const void *vp2)
{
    point *p1 = (point *)vp1;
    point *p2 = (point *)vp2;
    int orient = orientation(p0, *p1, *p2);
    if (orient == 0)
        return (dist(p0, *p2) >= dist(p0, *p1)) ? -1 : 1;
    return (orient == 1) ? -1 : 1;
}

int tangent(point *v, int vsize, point p)
{
    int l = 0;
    int r = vsize;
    int l_before = orientation(p, v[0], v[vsize - 1]);
    int l_after = orientation(p, v[0], v[(l + 1) % vsize]);
    while (l < r)
    {
        int c = (l + r) / 2;
        int c_before = orientation(p, v[c], v[(c - 1 + vsize) % vsize]);
        int c_after = orientation(p, v[c], v[(c + 1) % vsize]);
        int c_side = orientation(p, v[l], v[c]);
        if (c_before != RIGHT_TURN && c_after != RIGHT_TURN)
            return c;
        else if ((c_side == LEFT_TURN && (l_after == RIGHT_TURN || l_before == l_after)) ||
                 (c_side == RIGHT_TURN && c_before == RIGHT_TURN))
            r = c;
        else
            l = c + 1;
        l_before = -c_after;
        l_after = orientation(p, v[l], v[(l + 1) % vsize]);
    }
    return l;
}

void keep_left(point *v, int *vsize, point p)
{
    while (*vsize > 1 && orientation(v[*vsize - 2], v[*vsize - 1], p) != LEFT_TURN)
        (*vsize)--;
    if (*vsize == 0 || (v[*vsize - 1].x != p.x || v[*vsize - 1].y != p.y))
    {
        v[*vsize] = p;
        (*vsize)++;
    }
}

void GrahamScan(point *points, int n, point *hull, int *hullsize)
{
    if (n <= 1)
    {
        if (n == 1)
        {
            hull[0] = points[0];
            *hullsize = 1;
        }
        return;
    }
    p0 = points[0];
    for (int i = 1; i < n; i++)
    {
        if ((points[i].y < p0.y) || (points[i].y == p0.y && points[i].x < p0.x))
            p0 = points[i];
    }
    qsort(points, n, sizeof(point), compare);

    *hullsize = 0;
    for (int i = 0; i < n; i++)
    {
        keep_left(hull, hullsize, points[i]);
    }
    int t = *hullsize;
    for (int i = n - 2; i >= 0; i--)
    {
        keep_left(hull, hullsize, points[i]);
    }
    (*hullsize)--;
}

typedef struct
{
    point *pts;
    int size;
} Hull;

void chansalgorithm(point *v, int n, point *output, int *outsize)
{
    for (int t = 0; t < n; t++)
    {
        for (int m = 1; m < (1 << (1 << t)); m++)
        {
            int num_hulls = (n + m - 1) / m;
            Hull *hulls = (Hull *)malloc(sizeof(Hull) * num_hulls);
            point **hull_pts = (point **)malloc(sizeof(point *) * num_hulls);

            for (int i = 0; i < num_hulls; i++)
            {
                int start = i * m;
                int end = (start + m > n) ? n : (start + m);
                hull_pts[i] = (point *)malloc(sizeof(point) * (end - start));
                int hsize = 0;
                GrahamScan(v + start, end - start, hull_pts[i], &hsize);
                hulls[i].pts = hull_pts[i];
                hulls[i].size = hsize;
            }

            printf("\nM (Chunk Size): %d\n", m);
            for (int i = 0; i < num_hulls; i++)
            {
                printf("Convex Hull for Hull #%d:\n", i);
                for (int j = 0; j < hulls[i].size; j++)
                {
                    printf("(%d,%d) ", hulls[i].pts[j].x, hulls[i].pts[j].y);
                }
                printf("\n");
            }

            int h = 0, p = 0;
            for (int i = 0; i < num_hulls; i++)
            {
                for (int j = 0; j < hulls[i].size; j++)
                {
                    if (hulls[i].pts[j].y < hulls[h].pts[p].y)
                    {
                        h = i;
                        p = j;
                    }
                }
            }
            int hull_seq[1000][2];
            int hull_len = 0;
            hull_seq[hull_len][0] = h;
            hull_seq[hull_len++][1] = p;

            for (int i = 0; i < m; i++)
            {
                point curr = hulls[hull_seq[hull_len - 1][0]].pts[hull_seq[hull_len - 1][1]];
                int next_h = hull_seq[0][0];
                int next_p = (hull_seq[0][1] + 1) % hulls[next_h].size;
                point next = hulls[next_h].pts[next_p];

                for (int j = 0; j < num_hulls; j++)
                {
                    int idx = tangent(hulls[j].pts, hulls[j].size, curr);
                    point candidate = hulls[j].pts[idx];
                    int orient = orientation(curr, next, candidate);
                    if (orient == RIGHT_TURN || (orient == COLLINEAR && dist(curr, candidate) > dist(curr, next)))
                    {
                        next_h = j;
                        next_p = idx;
                        next = candidate;
                    }
                }

                if (next_h == hull_seq[0][0] && next_p == hull_seq[0][1])
                {
                    for (int k = 0; k < hull_len; k++)
                    {
                        output[k] = hulls[hull_seq[k][0]].pts[hull_seq[k][1]];
                    }
                    *outsize = hull_len;
                    for (int i = 0; i < num_hulls; i++)
                        free(hull_pts[i]);
                    free(hull_pts);
                    free(hulls);
                    return;
                }

                hull_seq[hull_len][0] = next_h;
                hull_seq[hull_len++][1] = next_p;
            }

            for (int i = 0; i < num_hulls; i++)
                free(hull_pts[i]);
            free(hull_pts);
            free(hulls);
        }
    }
}

int main()
{
    int T;
    scanf("%d", &T);
    if (T <= 0)
        return -1;
    point *points = (point *)malloc(sizeof(point) * T);
    for (int i = 0; i < T; i++)
    {
        scanf("%d%d", &points[i].x, &points[i].y);
    }
    point *output = (point *)malloc(sizeof(point) * T);
    int outsize = 0;
    chansalgorithm(points, T, output, &outsize);
    for (int i = 0; i < outsize; i++)
    {
        printf("(%d,%d) ", output[i].x, output[i].y);
    }
    printf("\n");
    free(points);
    free(output);
    return 0;
}
