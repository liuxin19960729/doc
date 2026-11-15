# c++必知必会
## 指南指针
### std::unique_ptr
```cpp

#include <memory>
对其持有堆内存的唯一拥有权(引用计数永远都是1)


初始化
1.
std::unique_ptr<int > p1(new int(10));

2.
std::unique_ptr<int> p2;
p2.reset(new int(10));

3.(c++14 添加进来的)
std::unique_ptr<int> p3 = std::make_unique<int>(10);


更加推荐第三种方式去创建std::unque_ptr



// 删除拷贝构造和复制操作
std::unique_ptr
   unique_ptr(const unque_ptr&) = delete;
   unique_ptr& operator=(const unque_ptr&) = delete;
   




// 移动构造
std::unique_ptr<int> p4(std::move(p2));
std::cout << *p4 << std::endl;
std::cout << p2.get() << std::endl;// nullptr


// 通过移动构造 创建一个新的unique_ptr
st::unque_ptr<int> p5(std::make_unique<int>(10));


note:
     只能指针在析构的时候只会释放其持有的堆内存(delete or delete[])



std::unque_ptr<T,Fun>=make_unique<T,Fun>(new T,Fun)
Fun：void(*)(T *):
    进行自定义的释放内存资源等操作



```

## std::shared_ptr
```cpp
共享指针 每共享一个 引用计数+1
每执行一次析构函数 数量 -1 并且 当计数 =0 则会释放内存

note:
    std::shared_ptr 并非线程安全


共享指针的三种处理方式

// c++11
std::shared_ptr<int> p1(new int(100));
std::cout << "p1:" << *p1 << " use_count:" << p1.use_count() << std::endl;
// c++11
std::shared_ptr<int> p2;
p2.reset(new int(200));
std::cout << "p2:" << *p2 << " use_count:" << p2.use_count() << std::endl;
// c++11
std::shared_ptr<int> p3 = std::make_shared<int>(300);
std::cout << "p3:" << *p3 << " use_count:" << p3.use_count() << std::endl;



如果不使用该指针 调用 引用计数会-1 or 释放内存


继承 std::enable_shared_from_this<T>

return shared_form_this() //返回 std::shared_ptr<T>




note:智能指针管理的是对对象 

```

## std::weak_ptr
```cpp
std::weak_ptr 作用 协助 std::shared_ptr 管理对象

std::weak_ptr 构造析构不会引起 计数的增加或减少



std::weak_ptr
    expired 方法检测是否失效
    
    lock() std::shared_ptr
      

note:
   std::weak_ptr 没有重写 operator* 和 operator-> 操作符
   比需使用 exipred 判断 lock 获取 在进行数据操作



```


## std::forward
```cpp
完美转发:在模版中将其参数原封不动地将将值类比传入给另一个函数(左值或右值)

```

## std::move
```cpp
无条件的将参数转换为右值引用


用在哪些地方:
    1.移动构造
    2.移动复制运算符

    

```
## c++ 规范
```cpp
c++11;
    return 返回一个局部变量(或函数参数),返回值类型和变量类型相同(或可转换)，并且存在可用的移动构造,编译器优先当做右值来处理隐式的执行 std::move




```