# libevent
```c
libevent:事件通知库,封装了reactor


reactor
io:(同步)
    io检测是否就绪:IO多路复用
    io操作 read/recv.....

事件:(异步)
    读事件
    写事件
    .....


io  检测IO是否就绪
io  操作IO

```
## libevent 的使用层次
```
1.基于libevent 对事件的处理我们自己做io处理

2.libevent内部对事件和io做了处理,我们自己只需要业务逻辑的处理



连接的建立
    接收连接  读事件
        accept
    主动连接  写事件
        connect

连接的断开
    主动断开 
    被动断开 读事件
        1.read() == 0
        2. write() ==-1  errno EPIPE
        3.EPOLLRDHUP(自己读端关闭) EPOLLHUP(读写都关闭)
数据的到达
    读事件
数据的发送
    send()/write() == -1 
    只有写失败才会注册写事件
    写事件触发将原来没有写入的数据 在此写入


```

## libevent 的封装层次
```
1.reactor 对象
    event_base
        eventop io多路复用








事件:
    1.网络事件
    2.信号事件
    3.定时事件
    
    

```