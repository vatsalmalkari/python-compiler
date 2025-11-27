#ifndef POINT_H
#define POINT_H

typedef struct {
    double x, y;
    void (*print)(void* self);
    double (*distance)(void* self);
} Point;

Point* Point_new(double x, double y);
void demo_oop();

#endif