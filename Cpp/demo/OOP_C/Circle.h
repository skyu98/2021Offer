#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"
typedef struct {
    Shape shape;
    int radius;
} Circle;

void Circle_ctor(Circle* const this, int x, int y, int r);

#endif // CIRCLE_H