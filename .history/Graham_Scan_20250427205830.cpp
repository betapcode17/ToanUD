#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Point
{
    int x, y;
};

typedef struct Point Point;

Point p0;

typedef struct
{
    Point *data;
    int top;
    int capacity;
} Stack;

Stack *createStack(int capacity)
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->data = (Point *)malloc(capacity * sizeof(Point));
    return stack;
}

bool isEmpty(Stack *stack)
{
    return stack->top == -1;
}

void push(Stack *stack, Point p)
{
    stack->data[++stack->top] = p;
}

Point pop(Stack *stack)
{
    return stack->data[stack->top--];
}

Point peek(Stack *stack)
{
    return stack->data[stack->top];
}

Point nextToTop(Stack *stack)
{
    Point p = pop(stack);
    Point res = peek(stack);
    push(stack, p);
    return res;
}

void swap(Point *p1, Point *p2)
{
    Point temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

int distSq(Point p1, Point p2)
{
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

int orientation(Point p, Point q, Point r)
{
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0)
        return 0;             // colinear
    return (val > 0) ? 1 : 2; // 1: clockwise, 2: counterclockwise
}

int compare(const void *vp1, const void *vp2)
{
    Point *p1 = (Point *)vp1;
    Point *p2 = (Point *)vp2;
    int o = orientation(p0, *p1, *p2);
    if (o == 0)
        return (distSq(p0, *p2) >= distSq(p0, *p1)) ? -1 : 1;
    return (o == 2) ? -1 : 1;
}

void GrahamScanAlgorithm(Point points[], int n)
{
    int ymin = points[0].y, min = 0;
    for (int i = 1; i < n; i++)
    {
        int y = points[i].y;
        if ((y < ymin) || (ymin == y && points[i].x < points[min].x))
        {
            ymin = points[i].y;
            min = i;
        }
    }

    swap(&points[0], &points[min]);
    p0 = points[0];

    qsort(&points[1], n - 1, sizeof(Point), compare);

    int m = 1;
    for (int i = 1; i < n; i++)
    {
        while (i < n - 1 && orientation(p0, points[i], points[i + 1]) == 0)
            i++;
        points[m] = points[i];
        m++;
    }

    if (m < 3)
        return;

    Stack *stack = createStack(m);
    push(stack, points[0]);
    push(stack, points[1]);
    push(stack, points[2]);

    for (int i = 3; i < m; i++)
    {
        while (stack->top > 0 && orientation(nextToTop(stack), peek(stack), points[i]) != 2)
            pop(stack);
        push(stack, points[i]);
    }

    while (!isEmpty(stack))
    {
        Point p = pop(stack);
        printf("(%d, %d)\n", p.x, p.y);
    }

    free(stack->data);
    free(stack);
}

int main()
{
    Point points[] = {{0, 3}, {1, 1}, {2, 2}, {4, 4}, {0, 0}, {1, 2}, {3, 1}, {3, 3}};
    int n = sizeof(points) / sizeof(points[0]);
    GrahamScanAlgorithm(points, n);
    return 0;
}
