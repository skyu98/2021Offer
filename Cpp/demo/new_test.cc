#include <iostream>
#include <string>
#include <new>
#include <vector>

using namespace std;

class Foo{
public:
    Foo() = default;
    ~Foo() {
        cout << "A Foo is destroyed..." << endl;
    }

    // plain new: 会抛出bad_alloc异常 new Foo();
    static void* operator new(size_t size) throw(std::bad_alloc) {
        cout << "Use Foo::operator new()!" << endl;
        return ::operator new(size);
    }

    // nothrow new: 不会抛出异常  new(nothrow) Foo();
    static void* operator new(size_t size, const nothrow_t& nothrow) noexcept {
        cout << "Use Foo::operator new() noexcept!" << endl;
        return ::operator new(size);
    }

    // placement new: 不申请内存，直接返回参数ptr
    static void* operator new(size_t size, void* ptr) noexcept {
        cout << "Use Foo::placement new()!" << endl;
        return ptr;
    }

private:
    int id;
    string name;
};

int main() { 
    
    vector<Foo*> vec;

    // 使用Foo::operator new
    vec.push_back(new Foo());
    vec.push_back(new (nothrow) Foo());

    // 使用全局new 
    vec.push_back(::new Foo());
    vec.push_back(::new (nothrow) Foo());

    for(Foo* p : vec) {
        delete p;
    }

    void* ptr1 = malloc(sizeof(Foo));
    void* ptr2 = malloc(sizeof(Foo));
    Foo* f1 = new(ptr1) Foo();
    Foo* f2 = ::new(ptr2) Foo();

    // placement new 不分配内存，所以不能使用delete；应该直接显式调用析构函数
    f1->~Foo();
    f2->~Foo();
    return 0;
}