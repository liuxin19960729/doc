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
[epoll](./epoll.c)
```cpp
#include <sys/epoll.h>

int epoll_create(int size);
   note: size 只需要大于0 就可以

int epoll_ctl(int epfd, int op, int fd,struct epoll_event *_Nullable event);
int epoll_wait(int n;int epfd, struct epoll_event events[n], int n,int timeout);



水平触发 (EPOLLLT) 默认
    只要状态在 就触发  比如 读写缓冲区有数据 就触发
    一次触发 读取一次数据 读取不完没有关系下一次还会触发
边缘触发(EPOLLET)
    只要状态改变 就触发 比如 读写缓冲区数据变化 就触发
    一次触发循环读    


```

## 服务器百万并发实现
```c
void *calloc(size_t count, size_t eltsize);


和 malloc 的区别
1.calloc 分配的内存 全部初始化为0




struct event_block
{
    struct sock_item *items;
    struct event_block *next
};



struct reactor
{
    int epfd;
    struct sock_item *items;
};



// 动态扩容数组
struct sock_item *items;
适用条件
1.有序,有规律


fd

五元组 (src ip  src port dst ip dst port proto)


src ip  src port(65535) dst ip  dst port(9999) proto(tcp)

src 是本机断开随机的
要让 无元组不通 可以 
  1.添加网卡 绑定不通的ip
  2.绑定多个端口





并发: 同时承载客户端的数量
  1.网络连接
  2.每个业务QPS
  3.断开连接的时候
  4.每一秒数据传输


100 W 并发决定的因素
1.epoll 100 万连接一点问题没有
```
## http
```
GET / HTTP/1.1\r\n
```
## websocket
![WebSocket协议详解信息图](./WebSocket协议详解信息图.png)
```

// openssl 库安装
sudo apt install libssl-dev


TCP 处理分包粘包方式
1.分隔符
2.长度+数据


HTTP 使用\r\n\r\n 分割符来处理粘报

// 区分http是否是websocket 连接协议
Connection: Upgrade
Upgrade: websocket


WebSocket 握手 ->TCP 三次握手 ->client socket http Connection: Upgrade Upgrade: websocket


握手流程
1.客户端发送
2.服务器响应
    1.请求 Sec-WebSocket-Key  值获取
    2.GUID	"258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
    3.Sec-WebSocket-Key值+GUID值连接
    4.SHA-1(Sec-WebSocket-Key值+GUID值)
    5.base65_encode(SHA-1(Sec-WebSocket-Key值+GUID值))
    6.响应 Sec-WebSocket-Accept 值 base65_encode(SHA-1(Sec-WebSocket-Key值+GUID值))


3.向客户端响应
HTTP/1.1 101 Switching Protocols
 Upgrade: websocket
 Connection: Upgrade
 Sec-WebSocket-Accept: xxxx




websocket 

1.握手
 判断是否存在  Sec-WebSocket-Key
  获取Sec-WebSocket-Key对应的值


   base64_encode(SHA-1(Sec-WebSocket-Key值+GUID值))  

2.数据传输格式


      0                   1                   2                   3
      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
     +-+-+-+-+-------+-+-------------+-------------------------------+
     |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
     |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
     |N|V|V|V|       |S|             |   (if payload len==126/127)   |
     | |1|2|3|       |K|             |                               |
     +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
     |     Extended payload length continued, if payload len == 127  |
     + - - - - - - - - - - - - - - - +-------------------------------+
     |                               |Masking-key, if MASK set to 1  |
     +-------------------------------+-------------------------------+
     | Masking-key (continued)       |          Payload Data         |
     +-------------------------------- - - - - - - - - - - - - - - - +
     :                     Payload Data continued ...                :
     + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
     |                     Payload Data continued ...                |
     +---------------------------------------------------------------+






状态机 WS_HANDSHARK WS_TRAMISSON WS_EN WS_COUNT


Payload len  < = 127










自定义协议注意
1.tcp分包粘包
2.数据
    json/xml/protobuf....






```