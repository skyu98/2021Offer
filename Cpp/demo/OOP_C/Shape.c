#include "Shape.h"
#include <assert.h>
static int Shape_area_(Shape const * const this); 
static void Shape_draw_(Shape const * const this);

void Shape_ctor(Shape* const this, int x, int y) {
    // 每个类有自己的虚表，里面是本类的虚函数实现
    static struct Shape_Vtbl const vtable = {
        &Shape_area_,
        &Shape_draw_
    };

    this->vptr = &vtable;
    this->x = x;
    this->y = y;
}

void Shape_moveBy(Shape* const this, int x, int y) {
    this->x += x;
    this->y += y;
}

int Shape_getX(Shape const * const this) {
    return this->x;
}

int Shape_getY(Shape const * const this) {
    return this->y;
}

Shape const* largestShape(Shape const *shapes[], int nShapes) {
    Shape const *s = (Shape *)0;
    int max = 0U;
    int i;
    for (i = 0; i < nShapes; ++i) {
        int area = Shape_area(shapes[i]);// 虚函数调用
        if (area > max) {
            max = area;
            s = shapes[i];
        }
    }
    return s;
}

void drawAllShapes(Shape const *shapes[], int nShapes) {
    int i;
    for (i = 0; i < nShapes; ++i) {
        Shape_draw(shapes[i]); // 虚函数调用
    }
}

// ---- Shape类自己的虚函数实现（抽象类，这里的两个函数应该是纯虚函数，不能被调用）
static int Shape_area_(Shape const * const this) {
    assert(0);
    return 0;
}

static void Shape_draw_(Shape const * const this) {  
    assert(0);
}
