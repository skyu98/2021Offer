#include "Rectangle.h"
#include <stdio.h>

static int Rectangle_area_(Shape const * const this);
static void Rectangle_draw_(Shape const * const this);

void Rectangle_ctor(Rectangle* const this, int x, int y, int w, int h) {
    // Rec类的虚函数表
    static struct Shape_Vtbl const vtable = {
        &Rectangle_area_,
        &Rectangle_draw_
    };

    // 首先调用基类的构造函数
    Shape_ctor(&this->shape, x, y);
    // 然后我们需要指向本类的虚函数表
    this->shape.vptr = &vtable;
    this->width = w;
    this->height = h;
}

// ----Rectangle类覆盖父类的虚函数，实现自己的版本
static int Rectangle_area_(Shape const * const this) {  // 注意这里传递的指针，既然是虚函数调用，那一定是通过父类指针来调用的
    Rectangle const * const this_ = (Rectangle const *)this; //显式的转换
    return (int)this_->width * this_->height;
}

static void Rectangle_draw_(Shape const * const this) {
    Rectangle const * const this_ = (Rectangle const *)this; //显示的转换
    printf("Rectangle_draw_ : (x = %d, y = %d, width = %d, height = %d)\n",
           Shape_getX(this), Shape_getY(this), this_->width, this_->height);
}
