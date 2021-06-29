#include "shared_ptr.h"
#include "unique_ptr.h"
#include <iostream>
#include <memory>
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
        sp_1->info();

        auto sp_2(sp_1);
        cout << sp_2.use_count() << endl;
        (*sp_2).info();
    
        auto sp_3 = sp_1;
        cout << sp_3.use_count() << endl;
        sp_3.get()->info();

        cout << "Move sp1 to sp4" << endl;
        auto sp_4 = std::move(sp_1);
        if(!sp_1) {
            cout << "sp1 now is nullptr" << endl;
            sp_4->info();
            cout << sp_3.use_count() << endl;
        }
    }

    Unique_ptr<Foo> up_1(new Foo(2, 'l'));
    up_1->info();
    auto up_2 = std::move(up_1);
    (*up_2).info();

    return 0;
}