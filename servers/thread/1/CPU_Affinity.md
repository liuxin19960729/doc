# CPU Affinity
```c
绑定CPU只在那个核心上运行


#define _GNU_SOURCE

#include <sched.h>


cpu_set_t?
假设CPU 有8个核心



cpu_set_td的操作

CPU_ZERO(set)	清空 CPU 集合
CPU_SET(cpu, set)	将 CPU 加入集合
CPU_CLR(cpu, set)	将 CPU 从集合中移除
CPU_ISSET(cpu, set)	判断 CPU 是否在集合中
CPU_COUNT(set)	统计集合中的 CPU 数量
    例如集合 {0,2,3} CPU_COUNT(set) 返回3

CPU_EQUAL(set1, set2)	判断两个集合是否相同
CPU_AND(dest, a, b)	两个集合求交集
CPU_OR(dest, a, b)	两个集合求并集
CPU_XOR(dest, a, b)	两个集合求异或



例如
CPU_SET(0, &cpuset);
CPU_SET(2, &cpuset);
CPU_SET(3, &cpuset);

{0,2,3}




cpu_set_t 如何与 CPU 绑定？



#define _GNU_SOURCE
#include <sched.h>

int sched_setaffinity(
    pid_t pid,
    size_t cpusetsize,
    const cpu_set_t *mask
);

pid:进程id 0表示当前进程
cpusetsize:cpu_set_t 大小
mask:cpu_set_t 对象


return 
    0 成功
    -1 并设置 errno



例如将 当前线程绑定到CPU 0

    cpu_set_t cpuset;

    // 清空 CPU 集合
    CPU_ZERO(&cpuset);

    // 选择 CPU 0
    CPU_SET(0, &cpuset);

sched_setaffinity(0, sizeof(cpuset), &cpuset)



// 也可获得CPU 亲和新的绑定信息
sched_getaffinity(0, sizeof(cpuset), &cpuset)
  
  CPU_ISSET(i, &cpuset) // 判断cpu 是否在该集合里面



// 进程设置请和性
sched_setaffinity
sched_getaffinity




#define _GNU_SOURCE
#include <pthread.h>
#include <sched.h>

int pthread_setaffinity_np(
    pthread_t thread,
    size_t cpusetsize,
    const cpu_set_t *cpuset
);


thread:线程Id
cpusetsize:cpu_set_t 大小
mask:cpu_set_t 对象


int pthread_getaffinity_np(pthread_t thread, size_t cpusetsize,
                                  cpu_set_t *cpuset);




```
## sched_getcpu
```c
int sched_getcpu(void);

当前线程正在运行CPU 编号

```