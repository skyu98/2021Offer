#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Empty
{

};

class A
{
    int i;
};

class B
{
    int i;
    char ch1;
};

class C
{
    int i;
    char ch1;
    char ch2;
};

class D
{
    char ch1;
    int i;
    char ch2;
};

class E
{
    int i;
    string name;
};

class F
{
    vector<int> vec;
};

class Ptr
{
    int* ptr;
};

// int: 4
// char: 1
// long long: 8
// class Empty: 1
// class A: 4
// class B: 8
// class C: 8
// class D: 12
// class E: 32
// class F: 24
// class Ptr: 8
int main()
{
    cout << "int: " << sizeof(int) << endl
        << "char: " << sizeof(char) << endl
        << "long long: " << sizeof(long long) << endl
        << "class Empty: " << sizeof(Empty) << endl
        << "class A: " << sizeof(A) << endl
        << "class B: " << sizeof(B) << endl
        << "class C: " << sizeof(C) << endl
        << "class D: " << sizeof(D) << endl
        << "class E: " << sizeof(E) << endl
        << "class F: " << sizeof(F) << endl
        << "class Ptr: " << sizeof(Ptr) << endl;

    return 0;
}