# 数据库
目前只接触了MySQL（关系型）和Redis（非关系型）

# 1.MySQL
## 1.主键
主键是关系表中记录的**唯一标识**。主键的选取非常重要：主键不要带有业务含义，而应该使用BIGINT自增或者GUID类型。主键也不应该允许NULL。

## 2.索引
MySQL官方对索引的定义为：索引（Index）是帮助MySQL高效获取数据的**数据结构。**

### 2.1 为什么使用索引
* 如果不使用索引：
    * 二分查找：要求数据有序
    * 二叉搜索树：要求数据构建为二叉搜索树
    * ......

* 使用索引，**使用```key```来指向或引用```data```**；而**索引使用更高级的数据结构，则可以提高搜索的效率**

* 对于主键，关系数据库会自动对其创建主键索引。使用主键索引的效率是最高的，因为**主键会保证绝对唯一**。

### 2.2 索引的类型
* 1. **B+树索引**
大体脉络：

|数据结构演化|优点|缺点|
|--|--|--|
|二叉搜索树|$O(logN)$内找到数据|可能不平衡|
|平衡二叉搜索树|查找效率更稳定|节点只储存一个数据，IO效率低|
|B树|按页读取，提高效率|数据分散在各节点，范围查找不方便|
|B+树|1.只保存Key，IO效率更高<br>2.数据全部在叶节点，且页之间有双向链表连接，便于范围查找|...|

* 2. **聚集索引**与**非聚集索引**

http://www.liuzk.com/410.html

* 3. **hash索引**
    * 检索时不需要类似B+树从根节点到叶子节点逐级查找，只需一次哈希算法即可定位，速度非常快

    * 不支持范围查询（散列函数后不再有序）

## 3.事务及其特性
事务（transaction）是由指逻辑上**对数据的的一组操作**，是不可分割的一个工作单位。

数据库事务可以确保**该事务范围内的所有操作都可以全部成功或者全部失败**。如果事务失败，那么效果就和没有执行这些SQL一样，不会对数据库数据有任何改动。

### 3.1 四大特性
* 原子性（Atomic）
    * 将所有SQL作为原子工作单元执行，要么全部执行，要么全部不执行
    * 实现原理：```undo log```

* 一致性（Consistent）
    * 事务完成后，所有数据的状态都是一致的

* 隔离性（Isonation）
    * 如果有多个事务并发执行，每个事务作出的修改必须与其他事务隔离
    * 实现原理：锁

* 持久性（Duration）
    * 事务完成后，对数据库数据的修改被持久化存储
    * 实现原理：```redo log```
