# pthread_mutex_t
```c

pthread_mutex_t mutex; //定义互斥锁对象




int pthread_mutex_init(
    pthread_mutex_t *mutex,
    const pthread_mutexattr_t *attr
);

初始化互斥锁

attr:
    NULL 默认互斥锁属性



return
    0 成功
    ！0 直接返回错误码





// 简单的初始化方式
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_lock()	加锁，必要时阻塞等待

一个线程获得了锁  如果其他线程也要获得同一个锁则需要等待



return
    0 成功
    ！0 直接返回错误码



pthread_mutex_unlock()	解锁


pthread_mutex_trylock()	尝试加锁，不阻塞等待
   尝试加锁 和  pthread_mutex_lock 最大区别不会阻塞


return 
    0 成功获取锁
    EBUSY 锁已经被其他线程占用
    非零其他值 其他错误


pthread_mutex_timedlock()	限时等待加锁
尝试获取锁没有获取锁等待 时间到了没有获取锁 返回



ETIMEDOUT c错误


pthread_mutex_destroy()	销毁互斥锁

销毁互斥锁





```