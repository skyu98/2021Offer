#ifndef SHAPE_H
#define SHAPE_H

#include <stdint.h>

// 封装：类 = 数据成员 + 方法

// 多态：虚函数表
struct Shape_Vtbl;

// 抽象类Shape
typedef struct {
    // 虚表指针
    struct Shape_Vtbl const* vptr;
    // 数据成员
    int x;
    int y;
}Shape;  

struct Shape_Vtbl {
    // 虚表里有两个函数指针，相当于这些函数被声明为virtual
    int (*area) (Shape const * const this);
    void (*draw) (Shape const * const this);
};

// -----普通方法-----
// 构造函数
void Shape_ctor(Shape* const this, int x, int y);
// 修改数据成员的方法
void Shape_moveBy(Shape* const this, int x, int y);
// 获取数据成员的方法
int Shape_getX(Shape const * const this);   // 注意第一个const：常量对象和非常量对象都可调用(该指针不会改变this的内容)
int Shape_getY(Shape const * const this);

// -----虚函数实现（传入的是Shape*指针，如果指向子类，怎么调用子类自己的虚函数）
static inline int Shape_area(Shape const * const this) {   // inline 减少调用时间开销；static inline 防止未生成内联函数时造成重复定义
   return (this->vptr->area)(this);
}

static inline void Shape_draw(Shape const * const this) {  
    (this->vptr->draw)(this);
}

// ------使用了虚函数的方法----
Shape const* largestShape(Shape const *shapes[], int nShapes);
void drawAllShapes(Shape const *shapes[], int nShapes);


#endif // SHAPE_H