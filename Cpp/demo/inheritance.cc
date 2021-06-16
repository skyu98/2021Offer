#ifndef INHERITANCE_H
#define INHERITANCE_H
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

    virtual int func3() {
        cout << "Base1 func3()" << endl;
    }

private:
    int b1_;
    int64_t tmp_;
};

class Base2 {
public:
    virtual int func1() {
        cout << "Base2 func1()" << endl;
    }

    virtual int func2() {
        cout << "Base2 func2()" << endl;
    }

private:
    int64_t b2_;
};

class Derived final: public Base1, public Base2 {
public:
    int func1() { // 编译器会自动加上virtual
        cout << "Derived func1()" << endl;
    }   
    int func2() {
        cout << "Derived func2()" << endl;
    }
private:
    int d_;
};

typedef int(*VFPTR)(); 

void printVtable(const VFPTR* table, int count) {
    // int i = 0;
    // VFPTR func = nullptr;
    // while(table) {
    //     
    //     func = *(table);
    //     cout << func << endl;
    //     func();
    //     ++table;
    // }
    for(int i = 0;i < count;++i) {
        printf("The addr of func %d is : 0x%0x\n", i, table[i]);
        table[i]();
    }
}

int main() {
    cout << "Size of Base1 :" << sizeof(Base1) << endl;
    cout << "Size of Base2 :" << sizeof(Base2) << endl;
    cout << "Size of Derived :" << sizeof(Derived) << endl;

    Derived d;
    Base1* p1 = &d;
    Base2* p2 = &d;

    cout << "Using base ptr: " << endl;
    p1->func1();
    p2->func2();

    cout << "Using vptr: " << endl;
    VFPTR* table1 = reinterpret_cast<VFPTR*>((*(int64_t*)(&d))); // 
    printVtable(table1, 3);  // 虚表中3个函数：Derived::func1(), Derived::func2() 和 Base1::func3()

    VFPTR* table2 = reinterpret_cast<VFPTR*>((*((int64_t*)(&d) + 3)));  // 注意offset，应该为sizeof(Base1) / sizeof(int64_t*)
    printVtable(table2, 2);  // 虚表中2个函数：Derived::func1(), Derived::func2()
}   

#endif // INHERITANCE_H