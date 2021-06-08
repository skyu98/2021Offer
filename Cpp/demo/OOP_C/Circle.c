#include "Circle.h"
#include <stdio.h>
static int Circle_area_(Shape const * const this);
static void Circle_draw_(Shape const * const this);

void Circle_ctor(Circle* const this, int x, int y, int r) {
    static struct Shape_Vtbl const vtable = {
        &Circle_area_,
        &Circle_draw_
    };

    Shape_ctor(&this->shape, x, y);
    this->shape.vptr = &vtable;  
    this->radius = r;
}

// ----Circle类覆盖父类的虚函数，实现自己的版本
static int Circle_area_(Shape const * const this) {
    Circle const * const this_ = (Circle const *)this;
    return this_->radius * this_->radius;
}

static void Circle_draw_(Shape const * const this) {
    Circle const * const this_ = (Circle const *)this;
    printf("Circle_draw_ : (x = %d, y = %d, radius = %d)\n",
           Shape_getX(this), Shape_getY(this), this_->radius);
}