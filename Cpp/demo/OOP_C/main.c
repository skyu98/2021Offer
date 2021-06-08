#include "Rectangle.h"
#include "Circle.h"
#include <stdio.h>

int main() {
    Rectangle r1, r2;
    Rectangle_ctor(&r1, 0, 0, 2, 3);
    Rectangle_ctor(&r2, 1, 1, 4, 6);

    printf("Rect r1(x = %d, y = %d, width = %d, height = %d)\n",
           Shape_getX(&r1.shape), Shape_getY(&r1.shape), r1.width, r1.height);
    printf("Rect r2(x = %d, y = %d, width = %d, height = %d)\n",
           Shape_getX(&r2.shape), Shape_getY(&r2.shape), r2.width, r2.height);

    Shape_moveBy(&r1.shape, -1, -1);  // 取shape成员的地址
    Shape_moveBy((Shape*)&r2, -1, -1); // 直接强转，因为shape是第一个成员，其地址就是Rec的首地址

    printf("Rect r1(x = %d, y = %d, width = %d, height = %d)\n",
           Shape_getX(&r1.shape), Shape_getY(&r1.shape), r1.width, r1.height);
    printf("Rect r2(x = %d, y = %d, width = %d, height = %d)\n",
           Shape_getX(&r2.shape), Shape_getY(&r2.shape), r2.width, r2.height);

    Circle c1, c2;
    Circle_ctor(&c1, 0, 0, 2);
    Circle_ctor(&c2, 1, 1, 3);

    Shape const * shapes[] = { &r1.shape, &r2.shape, &c1.shape, &c2.shape };
    Shape const * largest = largestShape(shapes, sizeof(shapes)/sizeof(shapes[0]));
    printf("largetsShape (x = %d, y = %d)\n", Shape_getX(largest), Shape_getY(largest)); 

    drawAllShapes(shapes, sizeof(shapes)/sizeof(shapes[0]));   
    return 0;
}