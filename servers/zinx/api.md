# api
## net
### ResolveTCPAddr
```golang
将address解析为*net.TCPAddr
func ResolveTCPAddr(network, address string) (*TCPAddr, error)

network:
    例如
    "tcp" "tcp4" "tcp6"
address:
    例如
    "127.0.0.1:8080"
    "[::1]:8080"
     "localhost:8080"
     
将address解析为*net.TCPAddr

```
### ListenTCP
```golang
创建TCP监听器用于AcceptTCP 接收客户端连接
func ListenTCP(network string, laddr *TCPAddr) (*TCPListener, error)


network:
    例如
    "tcp" "tcp4" "tcp6"
laddr:
    例如
    laddr,err:=ResolveTCPAddr("tcp4", "127.0.0.1:8080")

    or

    laddr:=&net.TCPAddr{
		IP:   net.ParseIP("127.0.0.1"),
		Port: 8080,
		Zone: "",
	}

    net.ParseIP("127.0.0.1")
    IPV4 和 IPV6都能够解析

    laddr:=&net.TCPAddr{
		IP:   net.IPv4(127,0,0,0,1),
		Port: 8080,
		Zone: "",
	}





```
### ErrClosed
```golang
对已经关闭的网络连接进行I/O操作返回该错误
errors.Is(err,net.ErrClosed)

例如:
    1.TCP 管理后,继续read write 会返回net.ErrClosed
    2.一个goroutine 正在读取 另一个goroutine 同事关闭 会返回net.ErrClosed
    3.网络监听已经关闭 继续Accept 会返回net.ErrClosed

    .....

```
## os

### Signal
```golang
表示操作系统或Go运行环境传递给程序的信号
type Signal interface {
	String() string
	Signal() // to distinguish from other Stringers
}


type Signal int




ch := make(chan os.Signal)

// 接收信号
sig:=<-ch


```

### siginal/Notify
```golang
监听OS 发送给当前进程的信号并且通过channel 通知Go程序
func Notify(c chan<- os.Signal, sig ...os.Signal)


例如:
ch := make(chan os.Signal, 1)

	signal.Notify(ch, os.Interrupt) // 监听中断信号

	fmt.Println("waits")

	sig := <-ch //如果有信号写入到ch 则立即返回 变为非阻塞

	fmt.Println(sig)

	fmt.Println("finish")


ch := make(chan os.Signal, 1)

note: 缓冲区大小为1 表示可以存入一个数据,这样即使接收者暂时没有准备好也不会导致notify 阻塞

```
### signal.Stop
```golang
取消通过 signal.Notify 在 ch注册的监听信号

```

## crypto/tls
### LoadX509KeyPair
```golang
从证书文件和私钥文件中加载 TLS 身份凭证


核心概念
certFile:证明服务器身份 包含服务器公钥以及域名信息
keyFile:服务器必须妥善保管的密钥，与证书中的公钥配对。
TLS：利用证书、密钥交换和对称加密等机制，为通信提供加密和身份验证

func LoadX509KeyPair(certFile, keyFile string) (Certificate, error)

参数
certFile: X.509 证书文件路径
    例如 :server.crt
keyFile:  与证书匹配的私钥文件路径
     例如:server.key
Certificate:加载得到的 TLS 证书对象
error: 加载失败错误



server.crt:
-----BEGIN CERTIFICATE-----
MIID...
这里是证书的编码内容
...
-----END CERTIFICATE-----

里面的内容包含:
1.服务器公钥。
2.证书对应的域名或其他身份信息。
3.证书有效期。
4.签发者及数字签名等信息


server.key

-----BEGIN PRIVATE KEY-----
MIIE...
这里是私钥的编码内容
...
-----END PRIVATE KEY-----


or

-----BEGIN RSA PRIVATE KEY-----
...
-----END RSA PRIVATE KEY-----

or
-----BEGIN EC PRIVATE KEY-----
...
-----END EC PRIVATE KEY-----




LoadX509KeyPair 加载的时候公钥和私钥必须配对否则返回错误


```

### Listen
```golang

TLS 加密的 TCP 服务端
func Listen(network, laddr string, config *Config) (net.Listener, error)

参数含义
network:
    "tcp"、"tcp4"、"tcp6"

laddr: string 类型
    例如
    ":8443"、"127.0.0.1:8443"

config:
    TLS 配置



note: 返回后的listener 是加密后的基类 Accept 连接Conn 也是加密封装的基类


```

## atomic
### AddUint64
```golang
原子地将 delta 加到 addr 指向的变量上，并返回加法完成后的新值
func AddUint64(addr *uint64, delta uint64) (new uint64)


适合多个 goroutine 并发修改同一个计数器


```
## http
###  ListenAndServe
```golang
监听指定的 TCP 地址，接收客户端的 HTTP 请求，并交给对应的处理器处理

func ListenAndServe(addr string, handler Handler) error

参数:
addr
    ":8080"
    "127.0.0.1:8080"


handler

type Handler interface {
	ServeHTTP(ResponseWriter, *Request)
}

类实现 ServeHTTP(ResponseWriter, *Request) 可以作为http 处理器


```
### ListenAndServeTLS
```golang
会使用 TLS 加密连接，通常需要服务器证书和对应的私钥。



```

### Shutdown
```golang

优雅关闭http服务器
func (s *http.Server) Shutdown(ctx context.Context) error


流程
1.开始关闭流程
2.不接收新TCP连接
3.关闭空闲连接
4.等待正在处理的请求完成
  并检测ctx 是否timeout
5.成功完成或超时

```