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