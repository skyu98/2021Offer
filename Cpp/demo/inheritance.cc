#include <iostream>

using namespace std;

class Base1 {
public:
    virtual int func1() {
        cout << "Base1 func1()" << endl;
    }

    virtual int func2() {
        cout << "Base1 func2()" << endl;
    }

private:
    int b1_;
    int64_t tmp_;
};

class Base2 {
public:
    virtual int func3() {
        cout << "Base2 func3()" << endl;
    }

    virtual int func4() {
        cout << "Base2 func4()" << endl;
    }

private:
    int64_t b2_;
};

class Derived final: public Base1, public Base2 {
public:
    virtual int func1() { // 对于重写函数编译器会自动加上virtual
        cout << "Derived func1()" << endl;
    }   
    // virtual int func2() {
    //     cout << "Derived func2()" << endl;
    // }
    virtual int func3() {
        cout << "Derived func3()" << endl;
    }
    // virtual int func4() {
    //     cout << "Derived func4()" << endl;
    // }
private:
    int d_;
};

typedef int(*VFPTR)(); 

void printVtable(const VFPTR* table, int count) {
    cout << "Print vfs :" << endl;
    for(int i = 0;i < count;++i) {
        printf("The addr of func %d is : %p\n", i, table);
        (*table)();
        ++table;
    }
}

int main() {
    cout << "Size of Base1 :" << sizeof(Base1) << endl;
    cout << "Size of Base2 :" << sizeof(Base2) << endl;
    cout << "Size of Derived :" << sizeof(Derived) << endl;

    Derived d;
    Base1* p1 = &d;
    Base2* p2 = &d;
    printf("Base1* p1 = &d : p1 is %p\n", p1);
    printf("Base2* p2 = &d : p2 is %p\n", p2);

    cout << "---------Using base ptr: " << endl;
    printf("p1->func1():\n");
    p1->func1();
    printf("p2->func3():\n");
    p2->func3();

    cout << "---------Using vptr: " << endl;
    VFPTR* table1 = reinterpret_cast<VFPTR*>((*(int64_t*)(&d))); // 虚表指针也是指针，占8字节
    printVtable(table1, 3);  // 虚表中3个函数：Derived::func1(), Base1::func2()， Derived::func3()(func3原本是重写Base2中的函数，也被合并到这里)

    VFPTR* table2 = reinterpret_cast<VFPTR*>((*((int64_t*)(&d) + 3)));  // 注意offset，应该为sizeof(Base1) / sizeof(int64_t*)
    printVtable(table2, 2);  // 虚表中2个函数：Derived::func3()（实际上被Thunk到主基类的虚表中） 和 Base2::func4()
}   