#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "point.h"

void Point_print(void* self) {
    Point* p = (Point*)self;
    printf("Point(%.1f, %.1f)\n", p->x, p->y);
}

double Point_distance(void* self) {
    Point* p = (Point*)self;
    return sqrt(p->x * p->x + p->y * p->y);
}

Point* Point_new(double x, double y) {
    Point* p = malloc(sizeof(Point));
    p->x = x;
    p->y = y;
    p->print = Point_print;
    p->distance = Point_distance;
    return p;
}