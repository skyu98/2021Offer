[TOC]
# C++ 和 C 的区别
* C++**几乎**是C的超集——C只有少量的特性是C++没有的（*变长数组VLA*）
<br>
* 早期C++的程序基本上可以翻译成C代码再编译——C++在语言层面上提供了更便捷的使用方法
<br>
* 面向对象和面向过程？ ——**OOP只是一种思想！并不是某种语言的特性！** C++、Java等语言只是**在语言层面很好地支持和包含了这种思想**。
<br>
* [**C也能实现封装、继承和多态！**](https://blog.csdn.net/onlyshi/article/details/81672279?utm_medium=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.baidujs&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.baidujs)

# 一、C++关键字

## const关键字
### 1.用于修饰对象
**核心：区分顶层const和底层const**
从变量名称开始从右往左读：
```cpp
int num_1 = 10;
int num_2 = 20;

int * const a = &num_1; // a是一个常量；指针常量；指向int的指针常量
num_1= 20;// (√)
a = &num_2;// (error!)a自身不能改变，但其指向的值可以改变--顶层const

const int * b = &num_2; // b是一个指针；指向int的指针；指向const int的指针
*b = 30; // (error!)b自身可以改变，但不能通过b改变其指向的值--底层const
b = &num_1; // (√)

const int* const c = b; // c是指向const int的指针常量；c自身不能改变；也不能通过c改变其指向的值

const int& d = num_2; // 声明引用的const都是底层const--不能通过d改变其值
```
**指向常量的指针和常量引用，只是限制其本身的可参与操作，不限制其指向的对象本身是常量**


### 2.用于函数
表示该类成员函数为只读函数，不会对该对象进行修改

```cpp
class Foo
{
public:
    void setId(int newId){ id_ = newId; }
    int getId() const { return id_; }

private:
    int id_;
}
```
前提：**类在成员初始化后，```this```指针构建完成；在调用其成员函数时，会隐式地将```this```指针作为第一个参数传入:**
```cpp
this->setId(1); // == setId(this, 1);
```
**在类成员函数加上const限定，等同于给```this```指针加上const限定，表示不会通过```this```指针去改变其指向的Foo对象--即该函数为只读函数。**
```cpp
int Foo::getId() const;// == getId(const Foo* this);
```
因此，要求对某些函数进行const重载，以适应不同对象的调用。

---

## static关键字
### 1.用于修饰对象
```cpp
static global_static_var = 1;

void foo()
{
    int local_var = 0;
    static local_static_var;
}
```
```static```修饰的对象：
* 生命周期为整个程序运行期间：```foo()```之后，```local_var```被销毁；```local_static_var```依旧存在（**但是可见域依然在函数内部**）

* 储存区域为静态数据区```(rodata)```；若没有初始值则自动初始化为默认值。

* 全局静态变量在```main```函数之前已经存在；局部静态变量在运行到该代码块时生成(只初始化一次）。


### 2.用于修饰普通函数

```cpp
static void foo()
{
    int local_var = 0;
}
 ```
```static```修饰的普通函数：
* 可见域为定义该函数的文件：可以防止与他人的函数重名。普通的inline函数一般可以搭配static使用。
https://zhuanlan.zhihu.com/p/132726037

### 3.用于修饰类成员对象、函数
```cpp
class Foo
{
public:
    Foo(): 
        id_(num_of_created_)
    {
        ++num_of_created_;
    }

    static int getNum()
    {
        return num_of_created_;
    }

private:
    int id_;
    static num_of_created_;
}

static int Foo::num_of_created_ = 0; // 该变量为Foo类共享，所以在类外定义
```
```static```修饰的类成员变量：
* 该类的所有对象共享，不依赖于对象存在

```static```修饰的类成员函数：
* 不依赖于对象存在，可直接通过类名调用；但是不能在其中使用类的非静态成员。
```cpp
int num = Foo::getNum();
 ```
---

## new/delete运算符
### 1.malloc/free
我们知道在c语言中，如果想在堆空间申请一片内存，则需要调用：
```c
void* malloc(size_t size);
```
失败会返回```NULL```；得到所需内存后，我们将其强转为所需的类型的指针；在结束使用后，需要调用：
```c
void free(void *ptr); // ptr为malloc所得到的指针
```
对申请的内存空间进行释放，避免内存泄漏。

### 2.与malloc/free的关系
```new/delete```运算符，在实现中调用了```malloc/free```，但是**通过编译器来进行了更多的更智能的操作**:
* 无需用户计算所需内存空间大小
* 转换为与所需类型严格匹配的指针
* 会调用构造/析构函数

具体过程如下所示：
* 不带有指针成员的类```class Complex```:
    * new
![avatar](./imgs/new_complex.jpg)
    * delete
![avatar](./imgs/delete_complex.png)

* 带有指针成员的类```class String```:
    * new
![avatar](./imgs/new_string.png)
    * delete
![avatar](./imgs/delete_string.png)

### 3. 与malloc/free的区别
通过以上可以发现，两者存在以下区别：
|特征|new/delete|malloc/free|
|:--:|:--:|:--:|
|本质|运算符|c库函数|
|申请内存所在空间|自由储存区|堆空间|
|分配成功|完整类型指针|void*|
|分配失败|bad_alloc异常|NULL|
|分配内存大小|编译器使用sizeof得出|用户计算并指定|
|处理数组|new[...] / delete[...]|用户计算并指定|
|扩充已分配的内存|无法直观处理|realloc|
|是否相互调用|可以调用|不能调用new/delete|
|分配时内存不足|客户可以定制处理|只能返回NULL|
|函数重载|允许|不允许|
|构造/析构函数|调用|不调用|

https://www.cnblogs.com/engraver-lxw/p/8600816.html

### 4.sizeof关键字确定所需空间大小
可以通过```sizeof()```来确定基本类型与类类型的大小。
其中基本类型大小会根据机器平台有变；**而类类型大小由类中所含类型和内存对齐原则决定**
```cpp
class A {// sizeof(A) = 4
    int i;
};

class B { // sizeof(B) = 8
    int i;
    char ch1;
};

class C { // sizeof(c) = 8
    int i;
    char ch1;
    char ch2;
};

class D { // sizeof(D) = 12
    char ch1;
    int i;
    char ch2;
};
 ```


### 5.malloc底层实现
* 1.当开辟的空间小于128K时，malloc 的底层实现是系统调用函数```brk()```，其主要移动指针```_enddata```(**此时的 ```_enddata```指的是 Linux 地址空间中堆段的末尾地址，不是数据段的末尾地址**)
<BR>
* 2.当开辟的空间大128K时，```mmap()```系统调用函数来在**文件映射区域空间**来开辟。

---
## virtual关键字
### 1.虚函数
```cpp
Base* p = new Derived();
p->func(arg); // (*(ptr->_vptr)[n](p, arg))
```
* 运行期动态绑定：
    * 在编译期**只能确定是Base*类型的指针变量**，不能确定到底指向的是何类型；
    * 在运行期，通过**虚函数表**来决定具体执行哪一个函数
<br>
* 虚函数表(**具体实现见第三章多态部分**)
    * 虚函数表的**地址**放在实例头部；通过首部的**虚函数表指针**获得虚函数表
    * 在虚函数中查找对应的函数实现
    * 未被覆盖的虚函数，则完全保留父类的
    * 覆盖了的虚函数，替代原有父类的
    * **每个类**都有自己的虚函数表（类似```static```成员）
<br>
* 虚构造函数
    * **构造函数不能是虚函数**
        * 虚函数是在**缺少对象信息的情况下**使用的，而构造函数需要明确调用者的类型

        * 在构造函数未执行完之前，对象并没有实例化，也就没有内存空间来存放虚函数表指针
<br>
    * **析构函数一定要是虚函数**
    否则通过父类指针指向的子类，**在析构时只会调用父类的析构函数，而不会调用自己的析构函数，造成内存泄漏**。

### 2.虚继承
虚继承用于解决多继承条件下的菱形继承问题（浪费存储空间、**存在二义性**）。

### 3.虚函数表的实现（⭐️）
[虚函数实现机理（在多继承部分有错误）](https://blog.csdn.net/haoel/article/details/1948051/?utm_medium=distribute.pc_relevant.none-task-blog-baidujs_baidulandingword-3&spm=1001.2101.3001.4242)
[虚函数实现机理（目前而言最正确的）](https://zhuanlan.zhihu.com/p/41309205)

**首先明确派生类的虚函数表的生成规则：**
* *将基类虚表中的内容**拷贝一份**放到子类虚表中*。
* 如果派生类重写了基类某个虚函数，用派生类自己的虚函数替换原先基类虚函数的入口地址（重写）。
* 如果派生类增加了新的虚函数**且有多个虚表（多重继承）**，将会把**新的虚函数地址增加到第一个虚表中**，按照其在类中声明次序依次增加到虚表的最后。
* 虚表存的是虚函数指针，不是虚函数，虚函数和普通函数一样的，都是存在代码段的，只是他的指针又存到了虚表中。另外对象中存的不是虚表，存的是虚表指针。

#### 3.1 单继承时的虚函数表
```cpp
class A {
public:
    virtual func1() {}
private:
    int a_;
};

class B : public A {
public:
    virtual func1() {}
private:
    int b_;
}
```
* A和B都有自己的虚函数表，表的地址放在每个对象的头部。
* A类的虚表是编译器构建，其中放有A类所有的**虚函数**地址。B类的虚表是**先拷贝了一份A类的虚表，并对其中有重写的虚函数进行覆盖**。
* 如果B类再派生出一个C类，C类的虚表也是先先拷贝了一份B类的虚表，并对其中有重写的虚函数进行覆盖——所以**单继承的结构中，每个派生类都只会有一个虚表**，也就只会有一个虚表指针放在对象的头部。

#### 3.2 多继承时的虚函数表
```cpp
class Base1 {
public:
    virtual int func1() {}
private:
    int b1_;
};

class Base2 {
public:
    virtual int func2() {}
private:
    int b2_;
};

class Derived : public Base1, public Base2 {
public:
    virtual int func1() {}
    virtual int func2() {}
private:
    int d_;
};  
```
* 按照派生类生成虚函数表的规则，Derived类将拷贝两个父类的虚表，并且分别覆盖其中被重写的虚函数。
* 在这里，**Derived类有两个虚表指针，分别放在两个sub-object的开头**（具体见实验）
* **多继承时，不同的Base类型指针指向的地址不再固定在对象起始处**，而是有对应的偏移。(**在向上转换时（子类转化为基类），编译器会自动给转换的基类指针加上对应基类在子类对象中的偏移位置，使得基类指针指向子类对象中对应基类的虚指针所在的位置。**)
* *更深一点*：Derived中的Base1在这里是**主基类**，它的**虚表中放有Derived所有的虚函数（包括重写Base2中的函数）**，而Base2的虚表中，放的是`Thunk Derived::func2()`，即会**地址偏移到Base1的虚表中调用函数**。
---
# 二、智能指针


怎么发现或者验证当前程序中出现了循环引用？


# 三、面向对象编程

## 继承

1. 子类的初始化列表中不能直接初始化父类的成员变量
```cpp
class Base {
public:
    Base(int id) : id_(id) {}

private:
    int id_;
};

class Derived : public Base {
public:
    Derived(int id, const string& name)
        : id_(id),    // error: class ‘Derived’ does not have any field named ‘id_’
          name_(name) {

    }

private:
    string name_;
};
```
在执行子类的初始化列表之前，会先执行父类的构造函数，所以这里不能二次初始化；**想在初始化列表初始化父类的成员变量，只需要在其中调用父类的构造函数：**
```cpp
Derived(int id, const string& name)
        : Base(id),   // no error
          name_(name) {

    }
```
