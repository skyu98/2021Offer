#include <iostream>
using namespace std;

class Foo {
public:
    Foo() {
        cout << "Foo ctor" << endl;
    }

    ~Foo() {
        cout << "Foo dtor" << endl;
    }
};

class Test {
public:
    Test(Foo* ptr, Foo& ref)
     : ptr_(ptr),
       ref_(ref) {
        cout << "Test ctor" << endl;
        cout << "ptr_ is at : " << ptr_ << endl;
        cout << "ref_ is at : " << &ref_ << endl;
    }

    ~Test() {
        cout << "Test dtor" << endl;
    }

private:
    Foo* ptr_;
    Foo& ref_;
};

int main() {
    Foo f1, f2;
    cout << "size of 'Test' is : " << sizeof(Test) << endl;
    Test* t = new Test(&f1, f2);
    delete t;
    cout << "t is deleted!" << endl;
    return 0;
}