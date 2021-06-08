#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shape.h"

// 继承自Shape
typedef struct {
    Shape shape;
    int width;
    int height;
} Rectangle;

void Rectangle_ctor(Rectangle* const this, int x, int y, int w, int h);

#endif // RECTANGLE_H