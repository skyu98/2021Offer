[TOC]
# C++ 和 C 的区别
* C++**几乎**是C的超集——C只有少量的特性是C++没有的（*变长数组VLA*）
<br>
* 早期C++的程序基本上可以翻译成C代码再编译——C++在语言层面上提供了更便捷的使用方法
<br>
* 面向对象和面向过程？ ——**OOP只是一种思想！并不是某种语言的特性！** C++、Java等语言只是**在语言层面很好地支持和包含了这种思想**。
<br>
* [**C也能实现封装、继承和多态！**](https://blog.csdn.net/onlyshi/article/details/81672279?utm_medium=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.baidujs&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.baidujs)

---

# 基础知识
## 1.函数的调用过程
**核心：`%rsp`栈顶指针的动态变化过程**

如函数`P`调用函数`Q`：
* `P`的栈帧中：
    * 首先**作为上一个函数的Callee**，P需要将**Callee寄存器中的数据push保存**
    * **作为Caller**，将自己需要保存的数据，保存在`Callee`寄存器中（**因为这样从Q中返回时保证不会被改写**）；不能保存在寄存器中的数据，就申请内存保存在栈上作为**局部变量**（不会对齐）。
    * 将需要传递的参数，保存在**参数寄存器(`%rdi, %rsi, %rdx, %rcx, %r8, %r9`)** 当中（即6个整形变量）；多出的参数也保存在栈上（**内存对齐**为8字节的倍数）。
    * `call Q `（**分为两步：1.将返回地址%rip压栈 2.jmp到Q**）
<br>

* `Q`的栈帧中：
    * 重复上述步骤；
    * 完成自己的函数任务
    * **pop恢复Callee寄存器中的数据**
    * `ret`指令：此时`%rsp`指向的就是之前压栈的**返回地址**，所以只需要从这里继续执行(**将该地址放入`%rip`寄存器**)就完成了返回(整形返回值放在`%rax`寄存器)

![avatar](./imgs/func_stack.jpg)

## 2. 编译与链接
* **.cc**->*预处理*->**.i** ：展开宏、删除注释等
* **.i**->*编译*->**.s** ：**词法、语法、语义分析**及**优化**
* **.s**->*汇编*->**.o(ELF种类之一)**：按照规则将**汇编语言翻译为机器指令**
**.o(ELF种类之一)**->*链接*->**可执行文件(ELF种类之一)** ：具体见下

### 目标文件(.o)
重点：
* 1. `ELF Header`(ELF文件头)：文件属性、系统属性、链接属性、**入口地址**
* 2. 包含当前源文件的已编译的机器代码: `.text`
* 3. 包含各种**堆上变量**：
        * 已初始化全局变量和静态变量：`.data`
        * **未初始化或初始化为0**的全局变量和静态变量：`.bss`（不占空间，使用时分配）
* 4. 符号表，**存放在程序中定义和引用的函数和非栈上变量的符号信息**：`.symtab` 
* 5. 段表（`Section Header Table`）：给出**不同段的大小和位置**等其他信息

符号表内容：
* 全局链接器符号：在**当前模块中定义，并能被其他模块引用的符号**。对应**非静态的函数和全局变量**。
* 外部链接器符号：在**别的模块中定义，并被当前模块引用的符号**。对应在**其他模块中定义的非静态的函数和全局变量**。（外部连接器符号也是全局连接器符号）
* 局部链接器符号：**只在当前模块定义和引用的符号**。对应于静态的函数和全局变量，这些符号在当前模块中任何位置都可见，但**不能被别的模块引用**。

### 链接
#### 1. 静态链接
* 1. 空间与地址分配（`VMA`虚拟地址空间）：
        * 扫描所有模块，收集所有符号到**全局符号表(此时符号的地址并未确定)**

        * **分别合并所有同类别的段**，计算其中各符号在**合并后的地址**，填入全局符号表

* 2. 符号解析与重定位
        * 依据**重定位表**获取需要重定位的**函数和变量符号**（即**采用绝对引用方式访问**的变量和函数）
        * 在**全局符号表**中寻找对应符号，**将其地址填入之前的“临时假地址”**

* 3. 缺点：
        * **每个进程都会保留一份副本，浪费空间**
        * 如果库中有**修复或者更新，整个程序需要重新链接**

#### 2. 共享库
为了解决静态链接的缺点，我们**希望一个库可以被多个进程共享**；但是怎么复用呢？显然我们**需要将物理内存中的共享库映射到每一个进程的虚拟地址空间**。
* 1. **固定装载地址（静态共享库）**
    * 每个进程在自己的虚拟地址空间，在固定的位置分配固定大小的块，然后`loader`每次都将库加载到这个位置。
    * 缺点：
        * 浪费空间：不使用某些库时，也会预留其位置
        * 难以管理：越界、更新等问题
<br>

* 2. **装载时重定位**
    * 每个进程都在不同的位置开辟空间，将库映射到这个位置，十分灵活
    * 绝对地址的确定也很简单，直接加上**偏移量即可（因为没有拆分合并，结构没变）**
    * 缺点：
        * 假如库中的某条指令需要库中某个全局变量的绝对地址，那么不同的进程，这条指令的内容也不一样（他们被加载到的位置不同，偏移量也不同）
        * 那么**指令部分（.text段）则也需要每个进程都有自己单独的副本**，这样违背了我们的初衷（**数据.data段当然是每个进程有自己的副本**）
<br>

* 3. **地址无关代码(PIC)**（**指令中不使用绝对地址**）
    * 模块内部调用/访问数据：由于**模块内部的偏移是确定的，所以直接通过偏移来实现**
    * 模块间调用/访问数据：
        * 在 **.data段有GOT全局偏移表**，里面保存有**其它模块中变量和函数的地址**
        * **指令中会先通过偏移找到GOT中的对应条目，然后通过地址取到对应的变量和函数**

#### 3. 动态链接
* `gcc --share -fpic -o libxx.so libxx.c -ldl` 生成地址无关的共享库
* 运行程序`program1`：
    * 首先系统会加载`program1.o`，发现有`.interp`段(需要进行动态链接)，`loader`依据其加载**dynamic linker**，交出控制权给**dynamic linker**
    * **dynamic linker**自己也是一个动态库，所以它需要完成**自举**
    * **dynamic linker**完成所有的动态链接工作（加载`libxx.so`到内存中，依据上述方式**完成重定位**等）
    * **交出控制权**给`program1`(即从入口地址开始执行)

* 此时如果还有`program2`、`program3`需要使用`libxx.so`，都**只需要完成后续的映射和重定位**即可。

#### 4. 显式运行时链接
* `dlopen()`加载动态库、`dlsym()`依据符号获取对应函数地址指针
* `dlerror()`检查加载错误、 `dlclose`卸载动态库

---

# 一、C++关键字

## const关键字
### 1.用于修饰对象
**核心**：区分**顶层const(*自己是const*)**和**底层const（*指向的object是const*）**
从变量名称开始**从右往左读**：
```cpp
int num_1 = 10;
int num_2 = 20;

int * const a = &num_1; // a是一个常量；指针类型的常量；指向int的指针类型的常量
num_1= 20;// (√)
a = &num_2;// (error!)a自身不能改变，但其指向的值可以改变--顶层const

const int * b = &num_2; // b是一个指针；指向int的指针；指向const int的指针
int const * b = &num_2; // b是一个指针；指向常量的指针；指向const int的指针（等价）
*b = 30; // (error!)b自身可以改变，但不能通过b改变其指向的值--底层const
b = &num_1; // (√)

const int* const c = b; // c是指向const int的指针类型的常量；c自身不能改变；也不能通过c改变其指向的值

const int& d = num_2; // 声明引用的const都是底层const--不能通过d改变其值
```
**指向常量的指针和常量引用，只是限制其本身的可参与操作，不限制其指向的对象本身是常量**

### 2.用于函数
表示该类成员函数为只读函数，不会对该对象进行修改

```cpp
class Foo {
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
因此，**要求对某些函数进行`const`重载**，以适应不同对象的调用(`const object`则调用`const`版本)。

---

## static关键字
### 1.用于修饰对象
```cpp
static global_static_var = 1; // 全局静态变量

void foo() {
    int local_var = 0;
    static local_static_var; // 局部静态变量；重入不再重新初始化（不在栈上）
}
```
`static`修饰的对象：
* 生命周期为整个程序运行期间：`foo()`之后，`local_var`被销毁；`local_static_var`依旧存在（**但是可见域依然在函数内部**）

* 储存区域为**静态数据区`(rodata)`**；若没有初始值则自动初始化为默认值。

* **全局静态变量**在`main`函数之前已经存在（`.init`段）；局部静态变量在运行到该代码块时生成(只初始化一次）。


### 2.用于修饰普通函数

```cpp
static void foo() {
    int local_var = 0;
}
 ```
```static```修饰的普通函数：
* 可见域为定义该函数的文件：可以防止与他人的函数重名。普通的`inline`函数一般可以搭配`static`使用。(**inline 减少调用时间开销；static inline 防止未生成内联函数时造成重复定义**)
https://zhuanlan.zhihu.com/p/132726037

### 3.🌟用于修饰类成员对象、函数
```cpp
class Foo {
public:
    Foo(): 
        id_(num_of_created_) {
        ++num_of_created_;
    }

    static int getNum() {
        return num_of_created_;
    }

private:
    int id_;
    static int num_of_created_;
}

static int Foo::num_of_created_ = 0; // 该变量为Foo类共享，所以在类外定义
```
```static```修饰的类成员变量：
* 该类的所有对象共享，不依赖于对象存在

```static```修饰的类成员函数：
* **不传入`this`指针**
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

失败会返回`NULL`；得到所需内存后，我们将其强转为所需的类型的指针；在结束使用后，需要调用：
```c
void free(void *ptr); // ptr为malloc所得到的指针
```
对**申请的内存空间**进行释放，避免内存泄漏（不是由`malloc()`申请的空间，则不能用`free()`释放）。

### 2.与malloc/free的关系
```new/delete```运算符，在实现中调用了```malloc/free```，但是**通过编译器来进行了更多的更智能的操作**:
* **无需用户计算**所需内存空间大小
    * 其实仍然调用了**sizeof()**
* 转换为与所需类型严格匹配的指针
    * 使用`static_cast<T*>(ptr)`
* 会**调用构造/析构函数**
    * 通过`placement new`调用构造函数
    * 如果是**non-trival**的析构函数，则会在**free()**前调用

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
|特征|`new/delete`|`malloc/free`|
|:--:|:--:|:--:|
|本质|运算符|`c`库函数|
|申请内存所在空间|自由储存区|堆空间|
|分配成功|完整类型指针|`void*`|
|分配失败|`bad_alloc`异常(可设置`handler`)|`NULL`|
|分配内存大小|编译器使用`sizeof`得出|用户计算并指定|
|处理数组|`new[...]` / `delete[...]`|用户计算并指定|
|扩充已分配的内存|无法直观处理|`realloc`|
|是否相互调用|可以调用|不能调用`new/delete`|
|分配时内存不足|客户可以定制处理|只能返回`NULL`|
|函数重载|允许重载<br>`operator new/delete`|不允许|
|构造/析构函数|调用|不调用|

https://www.cnblogs.com/engraver-lxw/p/8600816.html

具体的`operator new`重载请见[new_test.cc](./demo/new_test.cc)

### 4.sizeof关键字确定所需空间大小
可以通过`sizeof()`来确定**基本类型**与**类类型**的大小。
其中基本类型大小会根据机器平台有变；
**而类类型大小由类中所含类型和内存对齐原则决定:**
* 1. 结构体变量的**起始地址**能够被其最宽的成员大小整除

* 2. 结构体每个成员相对于**起始地址的偏移**能够被**编译器指定大小（默认为4）和其自身大小的最小值**整除，如果不能则在前一个成员后面补充字节

* 3. 结构体**总体大小**能够被最宽的成员的大小整除

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
* 1.当开辟的空间小于$128K$时，`malloc`的底层实现是系统调用函数`brk()`，其主要移动指针`brk`(**Linux 地址空间中堆段的末尾地址**)
<BR>

* 2.当开辟的空间大$128K$时，`mmap()`系统调用函数来在**文件映射区域空间**来开辟。
<br>

### 6. new[...] / delete[...]一定搭配使用
* 1. `new[]`在创建**POD类型**的数组时，不会有额外的记录（可以理解为**dtor是trival**的，直接`free`即可，**无需调用析构函数**），所以使用`delete`也不会有问题。

* 2. `new[]`在创建**类类型**数组时，会在返回的内存区域前**额外申请一块内存来保存数组元素个数**$n$，而在`delete[]`中，**指针可能会被调整为指向该内存块**，获取$n$并依次**调用$n$次析构函数**；因此**要不会造成内存泄漏、要不会运行时报错**。

---
## virtual关键字
### 1.虚函数
```cpp
Base* p = new Derived();
p->func(arg); // (*(ptr->_vptr)[n](p, arg))
```
* 运行期动态绑定：
    * 在编译期**只能确定是Base*类型的指针变量(按照Base的size去取对象)**，不能确定到底指向的是何类型；
    * 在运行期，通过**虚函数表**来决定具体执行哪一个函数
<br>

* 虚函数表(**具体实现见第三章多态部分**)
    * 虚函数表的**地址**放在**实例头部**；通过头部的**虚函数表指针**获得虚函数表
    * 在虚函数中查找对应的函数实现
    * 未被覆盖的虚函数，则完全保留父类的
    * 覆盖了的虚函数，替代原有父类的
    * **每个类**都有自己的虚函数表（类似`static`成员）
<br>
* 虚构造函数
    * **构造函数不能是虚函数**
        * 逻辑上：虚函数是在**缺少对象信息的情况下**使用的，而构造函数需要明确调用者的类型

        * 实现上：在构造函数未执行完之前，**对象并没有实例化**，也就没有内存空间来存放虚函数表指针

        * 执行上：构造对象时，会先调用父类的构造函数，此时该对象头部的虚函数指针指向的是父类的虚函数表，执行子类的虚函数无从谈起
<br>
    * **析构函数一定要是虚函数**
    * 否则通过父类指针指向的子类，**在析构时只会调用父类的析构函数，而不会调用自己的析构函数，造成内存泄漏**。

### 2.虚继承
虚继承用于解决多继承条件下的菱形继承问题（浪费存储空间、**存在二义性**）。

### 3.虚函数表的实现（⭐️）
[虚函数实现机理（在多继承部分有错误）](https://blog.csdn.net/haoel/article/details/1948051/?utm_medium=distribute.pc_relevant.none-task-blog-baidujs_baidulandingword-3&spm=1001.2101.3001.4242)
[虚函数实现机理（目前而言最正确的）](https://zhuanlan.zhihu.com/p/41309205)

**首先明确派生类的虚函数表的生成规则：**
* *将基类虚表中的内容**拷贝一份**放到子类虚表中*。
* 如果派生类重写了基类某个虚函数，用派生类自己的虚函数替换原先基类虚函数的入口地址（重写）。
* 如果派生类增加了新的虚函数**且有多个虚表（多重继承）**，将会把**新的虚函数地址增加到第一个虚表中**，按照其在类中声明次序依次增加到虚表的最后。
* **虚表存的是虚函数指针，不是虚函数**；**虚函数和普通函数一样的，都存在代码段，只是其指针又存到了虚表中**。另外对象中存的不是虚表，存的是虚表指针。

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
* A和B都有自己的虚函数表，表的**地址放在每个对象的头部**。
* A类的虚表是编译器构建，其中放有A类所有的**虚函数地址**。B类的虚表是**先拷贝了一份A类的虚表，并对其中有重写的虚函数进行覆盖**。
* 如果B类再派生出一个C类，C类的虚表也是先拷贝了一份B类的虚表，并对其中有重写的虚函数进行覆盖——所以**单继承的结构中，每个派生类都只会有一个虚表**，也就只会有一个虚表指针放在对象的头部。

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
* 按照派生类生成虚函数表的规则，`Derived`类将拷贝两个父类的虚表，并且分别覆盖其中被重写的虚函数。
* 在这里，**Derived类有两个虚表指针，分别放在两个sub-object的开头**（具体见实验）
* **多继承时，不同的Base类型指针指向的地址不再固定在对象起始处**，而是有对应的**偏移**。(**在向上转换时（子类转化为基类），编译器会自动给转换的基类指针加上对应基类在子类对象中的偏移位置，使得基类指针指向子类对象中对应基类的虚指针所在的位置。**)
* *更深一点*：`Derived`中的`Base1`在这里是**主基类**，它的**虚表中放有Derived所有的虚函数（包括重写Base2中的函数）**，而`Base2`的虚表中，放的是`Thunk Derived::func2()`，即会**地址偏移到Base1的虚表中调用函数**。这里会有一个专门的位置存放**往上的偏移大小**。

---
# 二、智能指针
[陈硕大佬的文章](https://blog.csdn.net/solstice/article/details/8547547)

![avatar](./imgs/shared_ptr.png)

## 1. 线程安全
* `shared_ptr`的**引用计数本身是安全且无锁（std::atomic::fetch_add）**的，但**对象的读写则不是**，因为`shared_ptr`有**两个数据成员，读写操作不能原子化（A线程尚未修改完毕，B线程对其进行了修改）。**

* `shared_ptr`的线程安全级别和内建类型、标准库容器、`std::string`一样

## 2.特点
* `ref_count`中也有`ptr`指向控制块；这使得**某个`shared_ptr<Foo>`转型之后得到的指针也能正确地管理Foo对象，而不会调用错误的析构函数**；因为`ref_count`中的ptr记住了**控制块的实际类型为Foo**

* 尽量使用`make_shared()`；这样可以节省一次内存分配，把**控制块和计数器所需的内存一起分配**。


## 3.循环引用
当多个对象分别持有指向对方的智能指针，则它们永远不会析构——这就是循环引用。
### 3.1 如何解决？
将某个对象中的`shared_ptr`改为`weak_ptr`，使其不增加引用计数，类似于打破死锁的循环等待。

### 3.2 如何发现？
* 工程上：使用`valgrind`工具查看**是否存在内存泄漏**。

* 理论上：对程序中持有`shared_ptr`的对象进行建有向图；进行**拓扑排序来验证是否有环存在**。

---
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

---
# 四、C++11新特性

## 1. 智能指针（见第二章）

## 2. `bind`原理及用法
http://blog.think-async.com/2010/04/bind-illustrated.html
* 场景一：作为**适配器**(`Adaptor`)统一`task`接口
```cpp
// 执行机构接受void(*)()类型的task
typedef std::function<void()> task; 
void Handler::handle(const task& t) {
    t();
}

void func1() {}

task t1 = func1; // 实际上是使用一个函数指针
handle(t1);

// 然而有时期待使用的参数，和接口要求的参数并不匹配
// 考虑这样一个函数，我们需要传入一个参数age
void func2(int age) {
    printf("%d\n", age);
}
// task t2 = bar; // error: 参数类型不匹配

int age = ::getAge();
// bind作为适配器（Adaptor），改变函数接口形式
task func2 = std::bind(bar, age); // func2是一个类型为void(*)()的仿函数，重载了operator ()
handle(func2); // OK
```

* 场景二：回调函数为**成员函数**时的参数绑定(**将this指针显式绑定**)
```cpp
class Foo {
// Foo接受void(*)(int)类型的回调函数
typedef std::function<void(int)> Callback; 
public:
    /* 设置回调函数 */
    void setCallback(const Callback& cb) { cb_ = cb; }
private:
    /* 在有新动作到来时，执行回调函数 */
    void onNewAction() {
        int age = ::getAge();
        cb_(age);
    }
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
```
* `bind`还涉及到很多细节和用法，比如`pass by ref`、 `private函数`的权限问题，都在`Burger`项目中有所见，这里不加深究。

## 3. `lambda`匿名函数（也是仿函数）
```cpp
// lambda func
auto func1 = [int& total, int offset](const Foo& elem)->void {
    total += elem.getVal() + offset;
};

// actually a class is generated
class _SomeCompilerGeneratedName_ {
public:
    _SomeCompilerGeneratedName_(int& total, int offset)
    : total_(total),
      offset_(offset) {

    }

    void operator() (const Foo& elem) {
        total_ += elem.getVal() + offset_;
    }
private: 
    int& total_; // Captured by ref 
    int offset_; // Captured by value
};

// equals
auto func2 =  _SomeCompilerGeneratedName_(total, offset);
func1(elem);
func2(elem);
```

## 移动语义与完美转发
### 一、左值与右值
* 左值与右值是**表达式/对象的属性**
* 左值使用的是**对象的身份（在内存中有明确的位置）**；右值使用的是**对象的值（对象在内存？寄存器？）**
```cpp
int p = 2; // p在内存中有专属位置，可由 &p 得到；2 在哪里（寄存器？内存？）
```
* 左值持久；**右值短暂**：**右值引用的对象即将被销毁，没有其他的用户；因此我们可以「窃取」它的资源**

### 二、std::move(...)实现移动语义
* **变量是左值**（离开作用域才销毁；而不是离开语句即销毁）；因此我们**不能直接把一个右值引用绑定到变量上**
```cpp
int&& rRef = 42; // ok;42 is a rvl
int&& rRef2 = rRef; // error: rRef is lvl
```
* 当我们想**按照右值的方式来处理一个变量**时（即我们**想「窃取」其资源时**），可以使用`std::move()`**将某个变量转换为对应的右值引用类型**
```cpp
int&& rRef = 42; // ok;42 is a rvl
int&& rRef2 = std::move(rRef); // ok: std::move(rRef) returns a rvl
```
* 调用`std::move()`之后，**不能对移动后源对象**的值作任何假设

### 三、移动构造函数/移动赋值函数来完成「窃取」
* 调用`std::move()`只是**让源对象有被窃取的资格**
* 真正的资源窃取在**移动构造函数/移动赋值函数**中完成
* **移动构造函数/移动赋值函数**要**保证源对象可以安全析构**
* 由于**资源的转移不涉及申请新的空间**，因此**移动语义的函数应该是`noexcept`的**

### 四、引用折叠
* 一般情况下我们**不能定义引用的引用**（引用本身只是种方式而不是对象，方式如何叠加？）；但有如下两种特殊情况：
```cpp
/* 1.类型别名 */
typedef int& ref;
int a = 42;
ref& r = a;  // 展开为int& & r = a;

/* 2.模版类型参数 */
template <typename T> 
void func(T&&) {
    // ...
}
func(a); // 传入左值，T被推断为int&，则函数参数类型为int& &&
```
* 看上去我们生成了**引用的引用**，实际上发生了**引用折叠**
    * `T& &`, `T& &&`, `T&& &`折叠为:`T&`
    * `T&& &&`折叠为:`T&&`

* 🌟 对于`void func(T&&)`，我们**既可以传入左值（`T`被推断为`X&`），也可以传入右值（`T`被推断为`X`）**

### 五、完美转发
* 很多函数当中，**需要在不改变参数的类型信息的前提下转发参数**

* 简单地使用`T&&`参数类型**只能解决左值的问题，不能解决右值的问题；因为实例化出来的形参本身是一个左值（虽然它是右值引用）**

* `std::forward<T>()`能够返回`T&&`；**因此对于`T&&`参数类型：**
    * 如果传入右值，则`T`为`X`，返回`X&&`
    * 如果传入左值，则`T`为`X&`，返回`X& &&`，**折叠**为`X&`

* 因此，`std::forward<T>()`可以**完美地保存参数的类型信息**