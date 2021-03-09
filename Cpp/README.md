[TOC]

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
* 生命周期为整个程序运行期间：```foo()```之后，```local_var```被销毁；```local_static_var```依旧存在

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
* 可见域为定义该函数的文件：可以防止与他人的函数重名。

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
class A   // sizeof(A) = 4
{
    int i;
};

class B   // sizeof(B) = 8
{
    int i;
    char ch1;
};

class C   // sizeof(c) = 8
{
    int i;
    char ch1;
    char ch2;
};

class D   // sizeof(D) = 12
{
    char ch1;
    int i;
    char ch2;
};
 ```

---
# 二、智能指针
怎么发现或者验证当前程序中出现了循环引用？
# 三、
