# pthread_spinlock_t
```c
POSIX 库提供的自旋锁



#include <pthread.h>

int pthread_spin_init(
    pthread_spinlock_t *lock,
    int pshared
);


初始化自旋锁

lock:自旋锁对象
pshared 自旋锁共享范围
    PTHREAD_PROCESS_PRIVATE 
        只能在同一进程线程间共享
    PTHREAD_PROCESS_SHARED
        可以在多进程之间共享


return 
     0 成功
     非0  范湖错误码

pthread_spin_lock()	获取自旋锁
pthread_spin_trylock()	尝试获取自旋锁

    尝试获取锁 获取不到立即返回


return 
    0 成功
    EBUSY 锁已经被其他线程占用
    非0 其他错误


pthread_spin_unlock()	释放自旋锁
pthread_spin_destroy()	销毁自旋锁

```