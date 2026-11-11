# IO复用select和poll函数
```
一个或多个IO满足条件(fd对应的缓冲区现在有数据可以去读取数据或fd对应的缓冲去现在有空闲位置可以写入数据到缓冲区),就通知我们可以做对应的操作
```
## I/O模型
### 阻塞I/O模型
### 非阻塞I/O模型
```c
非阻塞调用如果系统没有数据或没有空位置则返回 -1 errno 并设置为  EAGAIN or EWOULDBLOCK
```
### I/O复用模型
### 信号驱动I/O模型
```c
内核在描述符准备好时用信号SIGIO通知我们

```
### 异步I/O模型
```c
1.内核拷贝数据到我们自己缓冲区拷贝好了通知我们
2.内核将我么缓冲区拷贝到内核缓冲区 拷贝好了通知我们
```
### 各种I/O模型的比较
```c
同步IO与异步IO
1.同步I/O引起请求经常阻塞直到完成
2.异步I/O 不引起进程阻塞
```
## select
```c

#include <sys/select.h>

typedef /* ... */ fd_set;

int select(int nfds, fd_set *_Nullable restrict readfds,
                  fd_set *_Nullable restrict writefds,
                  fd_set *_Nullable restrict exceptfds,
                  struct timeval *_Nullable restrict timeout);

 void FD_CLR(int fd, fd_set *set);
 int  FD_ISSET(int fd, const fd_set *set);
 void FD_SET(int fd, fd_set *set);
 void FD_ZERO(fd_set *set);




select 阻塞函数 只有当有事件发生 or 设置超时 超时的时候才会返回



return 
    >0 nready 数字
    =0 超时
    <0 错误


timeout:
struct timeval {
    time_t      tv_sec;         /* seconds */
    suseconds_t tv_usec;        /* microseconds */
};


struct timeval 设置超时的时间

1.timeout 设置为NULL 永远等下去直到有事件发生
2.timeval 设置 等待固定时间返回 或遇到 事件立即返回
3.tv_sec 和 tv_usec 设置为 0 不等待立即返回



```