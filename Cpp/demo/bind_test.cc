#include <functional>
int getAge() {
    static int age = 1;
    return age++;
}

// Foo接受void(*)(int)类型的回调函数
typedef std::function<void(int)> Callback; 

class Foo {
public:
    /* 设置回调函数 */
    void setCallback(const Callback& cb) { cb_ = cb; }

    /* 在有新动作到来时，执行回调函数 */
    void onNewAction() {
        int age = ::getAge();
        cb_(age);
    }
    
private:
    Callback cb_;
};

class Bar {
public:
    void MemberFunc(int age) {
        printf("MemFunc: %d\n", age);
    }
};

void func1(int age) { 
    printf("Func: %d\n", age); 
}

int main() {
    Foo foo;

    foo.setCallback(func1); // 直接使用函数指针，ok
    foo.onNewAction();

    /*  使用std::placeholders代替具体参数，代表这个参数是调用时即提供；
        而不是作为task时常见的，bind时即提供全部参数
    */
    foo.setCallback(std::bind(func1, std::placeholders::_1)); // 使用bind
    foo.onNewAction();

    // 如何回调Member function呢？
    Bar bar;
    // Bar::MemberFunc的类型是void(Bar::*)(int)，隐式包含一个this指针
    Callback cb = std::bind(&Bar::MemberFunc, &bar, std::placeholders::_1);
    foo.setCallback(cb);
    foo.onNewAction();
}