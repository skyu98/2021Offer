#include "shared_ptr.h"
#include "unique_ptr.h"
#include <iostream>
using namespace std;

struct Foo {
    Foo() {
        cout << "Foo is created" << endl;
    }
    Foo(int i, char n) : id(i), name(n) {
        cout << "Foo is created with info" << endl;
    }
    ~Foo() {
        cout << "Foo is deleted" << endl;
    }

    void info() const {
        cout << "Id is " << id << " and name is " << name << endl; 
    }
    int id;
    char name;
};

int main(){
    {
        Shared_ptr<Foo> sp_1(new Foo(1, 'h'));
        cout << sp_1.use_count() << endl;

        auto sp_2(sp_1);
        cout << sp_2.use_count() << endl;
    
        auto sp_3 = sp_1;
        cout << sp_3.use_count() << endl;

        sp_1->info();
        (*sp_2).info();
    }

    Unique_ptr<Foo> up_1(new Foo(2, 'l'));
    up_1->info();
    auto up_2 = std::move(up_1);
    (*up_2).info();
    return 0;
}