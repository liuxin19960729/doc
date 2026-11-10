# 基本套接字编程
## socket
```c
#include <sys/socket.h>
int socket(int domain, int type, int protocol);


domain;
    AF_INET
type:
    SOCK_STREAM


AF_INET/AF_INET6 + SOCK_STREAM = TCP
AF_INET/AF_INET6 + SOCK_DGRAM = UDP
```
## connect
```c
#include <sys/socket.h>
int connect(int sockfd, const struct sockaddr *addr,
socklen_t addrlen);

return 
  -1

返回错误的情况:
errno
1.
    ETIMEDOUT
    连续多次发送syn 都没有ACK 回应

2.
    ECONNREFUSED
    对客户端SYN 回应RST 表示服务端没有启动
3.  (Network is unreachable.)
    ENETUNREACH
    发送SYN 中间路由器返回ICMP错误,客户端报错该错误 继续发送SYN
    .... 直到超过次数和时间 返回



connect() >0  ESTABLISHED  
<0 CLOSED  fd 必须关闭

```
## bind
```c
#include <sys/socket.h>
int bind(int sockfd, const struct sockaddr *addr,
                socklen_t addrlen);



bind 可以将IP和端口地址绑定到套接字上,这个IP就是发送IP数据报的源IP和源端口



addr
  port 0 内核随机选择端口
  port !=0 内核就用用户设置的port


  IPV4 的通配地址为 ADDR-ANY


  IPV6
     <netinet/in.H>
     in6addr_any 表示通配地址


如果将port 设置为 0,我们想获取系统分配的端口号,可以通过getsockname 获取


#include <sys/socket.h>
#include <netinet/in.h>

// 获取 socket 的 local address
int getsockname(
    int sockfd,
    struct sockaddr *addr,
    socklen_t *addrlen
);



// 获取socket 的 peer address
int getpeername(int sockfd, struct sockaddr *restrict addr,
                       socklen_t *restrict addrlen);


1.
每个进程bind() 非通配符IP
    内核根据目的IP交给不同的进程

2.
    bind INADDR_ANY

    服务器调用 getsockname() 获取连接目的的IP




note:
    在 client:
        getsockname 返回的是客户端ip+port
        getpeername 返回的是服务器ip+listen port

    在 server:
        getsockname 返回的是服务器目的ip+listen port
        getpeername 返回的是客户端ip+port

```

## listen
```c
#include <sys/socket.h>
int listen(int sockfd, int backlog);

listen 将sockfd 未连接的套接口转换为被动套接口,sockfd CLOSED 状态变化为LISTEN


SYN-------------> 进入未完成连接队列
<---------------- SYN +ACK
ACK-----------------> 已完成连接队里


未完成连接队列里面的条目
1.超时未响应ACK 改条目从未完成队列删除
2.ACK 响应 该条目从未完成队里移至已完成队列


backlog // 并没有官方对该参数正式的定义
    man listen 
    指的是待处理连接()


accept() 如果是阻塞
   已完成队列如果是空的阻塞直到有连接进入



如果队里满的 客户端发送SYN
    POSIX 允许
    1.响应RST
        这种情况客户端不能判断是没有此应用RST 还是服务端的队列满了
    2.不做任何响应 等待客户端再次尝试发送SYN
```
## accept 函数
```c
#include <sys/socket.h>
int accept(int sockfd, struct sockaddr *_Nullable restrict addr,socklen_t *_Nullable restrict addrlen);

从已完成队列中取出一个连接

sockaddr
addrlen
    返回客户端的地址和端口

note:如果我们对对端的地址和端口不感兴趣,可以设置为NULL


return 
    > 0 新连接clienfd



```

## fork 和 exec
```c
#include <unistd.h>
pid_t getpid(void);
pid_t getppid(void);
pid_t fork(void);
该函数式派生新进程的唯一方法



return
    0 子进程返回
    >0 父进程返回
       返回的值是子进程的id
       如果父进程想跟踪子进程 必须记住返回的id
    -1 失败


子进程只有一个父进程
    getppid() 获取父进程id




fork() 之后 
   子进程共享父进程fork() 之前打开的所有文件描述符



clientfd=accept()
pid=fork()// 子进程共享fork() 之前打开的所有文件描述符

if(pid==0){
    // clientfd 读写
}else {
    close(clienfd)
}




```