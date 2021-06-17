#include <iostream>
using namespace std;

class Base {
public:
    virtual int func1() {
        cout << "Base func1" << endl;
    }
private:
    int64_t base_;
};

class A : virtual public Base {
    int64_t a_;
};

class B : virtual public Base {
    int64_t b_;
};

class C : public A, public B {
    int64_t d_;
};

int main() {
    cout << "Size of Base :" << sizeof(Base) << endl;
    cout << "Size of A :" << sizeof(A) << endl;
}
