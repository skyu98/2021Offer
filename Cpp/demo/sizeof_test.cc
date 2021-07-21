#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Empty {

};

class Ref {
public:
    Ref() = delete;
    Ref(char& ch) : ch1('0'), ch2(ch) {}
private:
    char ch1;
    char& ch2; // 被看作指针
};

class Ptr {
    int* ptr;
};

class A {
    int i;
};

class B {
    int i;
    char ch1;
};

class C {
    int i;
    char ch1;
    char ch2;
};

class D {
    char ch1;
    int i;
    char ch2;
};

class E {
    int i;
    string name;
};

class F {
    vector<int> vec;
};

// int: 4
// char: 1
// long long: 8
// class A: 4
// class B: 8
// class C: 8
// class D: 12
// class E: 32
// class F: 24
// class Empty: 1
// class Ref: 16
// class Ptr: 8
int main()
{
    cout << "int: " << sizeof(int) << endl
        << "char: " << sizeof(char) << endl
        << "long long: " << sizeof(long long) << endl
        << "class A: " << sizeof(A) << endl
        << "class B: " << sizeof(B) << endl
        << "class C: " << sizeof(C) << endl
        << "class D: " << sizeof(D) << endl
        << "class E: " << sizeof(E) << endl
        << "class F: " << sizeof(F) << endl
        << "class Empty: " << sizeof(Empty) << endl
        << "class Ref:" << sizeof(Ref) << endl
        << "class Ptr: " << sizeof(Ptr) << endl;

    return 0;
}