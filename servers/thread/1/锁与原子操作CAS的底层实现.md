# 锁与原子操作CAS的底层实现

## pthread_create
```c
线程创建


#include <pthread.h>

int pthread_create(
    pthread_t *thread,
    const pthread_attr_t *attr,
    void *(*start_routine)(void *),
    void *arg
);



thread:保存线程Id
attr:线程属性  线程属性，设置栈大小、分离状态等
start_routine:新线程启动执行的函数
arg 传递给线程的参数


return
    0:成功
    非0:失败 直接返回错误码
          trerror(ret) 获取错误原因
 



pthread_self() // 获取当前线程Id

pthread_equal(t1,t2) // 比较两个线程
        note:不是在所有平台线程都是整数不要使用==比较线程


const pthread_attr_t *attr,


pthread_attr_t attr;
pthread_attr_init(&attr);


默认
线程是 joinable（可连接）的。
    需要通过 pthread_join() 回收线程资源


// 设置线程分离不获取返回值

pthread_attr_setdetachstate(
    &attr,
    PTHREAD_CREATE_DETACHED
);

PTHREAD_CREATE_JOINABLE	可连接，默认状态
PTHREAD_CREATE_DETACHED	分离状态，线程退出后自动释放线程资源


使用系统默认栈大小。
    pthread_attr_setstacksize(&attr, 1024 * 1024);



note:
    栈大小需要满足 PTHREAD_STACK_MIN 的要求，实际还受到系统和平台限制

使用默认调度属性。
使用默认的线程栈地址分配方式。






note:
pthread_create 内核创建tast_struct 并将该结构加入到就绪队列
具体什么时候执行由调度器决定


```

## pthread_join
```c
阻塞等待退出并获取退出结果


#include <pthread.h>

int pthread_join(
    pthread_t thread,
    void **retval
);


thread:等待线程Id
retval 接收线程的返回值

return 
     0 成功
     非0 失败 直接返回错误码
          trerror(ret) 获取错误原因



note:
    线程执行完 joinable 线程任然保留部分线程资源和退出结果等待其他线程执行

    默认线程是joinable

```

## pthread_detach
```c
将线程设置为分离状态,线程退出系统回收它的线程资源,其他线程不允许对它使用pthread_join()



#include <pthread.h>

int pthread_detach(pthread_t thread);

```

## 原子操作
```c
i++;


// 汇编代码
mov [i],%eax; //加栈内存数据到寄存器
add $1,%eax;// 加1
mov %eax,[i];// 写到栈内存



怎么解决上面i++ 多线程问题
1.互斥锁 mutex
2.自旋锁 spin_lock 
3.原子操作
  stdatomic.h c11 标准库
  // gcc 
  __atomic_fetch_add(pcount, 1, __ATOMIC_RELAXED); 



```

## CPU 粘合
[CPU_Affinity](./CPU_Affinity.md)
```c
进程或线程只在设置的粘合的CPU上运行
```

## 大文件如果快速读到内存里面
```c
1.mmap 映射文件到内存
2.多线程操作映射出来的内存



共享内存 实现的方式
1.mmap

void *mmap(void addr[length], size_t length, int prot, int flags,int fd, off_t offset);


fd:映射的那个文件
offset:映射文件的偏移量
length 分配空间大小



return 
      是映射的地址

2.share memory 

```