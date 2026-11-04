# socket2
```cpp
bind address
1.127.0.0.1 绑定是回环地址 只能在本机两个进程间通信
2.192.168.x.x 具体地址 绑定的是固定网卡的地址 只能请求该网卡ip 才能连接
3.0.0.0.0 绑定的是通配符地址  绑定本机多个网卡 任意网卡都能连接通信



htonl(INADDR_ANY) 本机字节序转为网络字节序


linux 设置非阻塞

void setFDBlock(int fd, int block)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (block)
    {
        /* code */
        flags &= ~O_NONBLOCK;
    }
    else
    {
        // 设置非BLOCK
        flags |= O_NONBLOCK;
    }
    fcntl(fd, F_SETFL, flags);
}


1.多线程 多进程
2.多路IO复用



int ret = recv;
ret==0 表示客户端调用close 关闭

```
**一请求一线程**
[code](./one_request_one_thread.c)
```
1.逻辑简单
2.缺点 线程占用资源多 代价太大,客户端并发不会太大



```

## IO 多路复用
```
作用:检测fd 是否有事件(读写....)

```
### select
[select](./select.c)
### epoll
```

```