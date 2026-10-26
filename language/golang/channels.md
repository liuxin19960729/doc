# channels
```golang
Goroutine 是 Golang 语言的并发体,channels 则是 Goroutine 之间的通信机制.


chan T T表示Channel的类型 用于发送接收数据的类型


创建一个channel

ch:=make(chan T)
ch:=make(chan T,cap) cap 表示缓冲去大小

ch <- value 发送
x = <- ch 接收
<-ch 接收 值丢弃


close(ch) 关闭
    note: 
        1.对关闭的通道发送数据会导致panic 异常
        2.如果一个ch 已经关闭 但是里面仍然有数据 ,仍然可以读取到里面的数据
        3.读取一个已经关闭的ch,并且没有数据则参数零值数据


```
## 不带缓冲去的Channels
```golang
1.无缓存Channel 发送数据导致阻塞 直到另一个Goroutine在相同Channel 接收数据

2.无缓存Channel 接收数据导致阻塞 直到另一个Goroutine在相同Channel 发送数据




```

```
value,ok:=<-ch
ok 可以判断ch 是否关闭

```

## 查询通道 cap 和 当前有多少个元素
```golang
ch1 := make(chan int)

fmt.Println("ch1 len:", len(ch1), "cap:", cap(ch1))
ch2 := make(chan int, 100)
ch2 <- 1
ch2 <- 2
fmt.Println("ch2 len:", len(ch2), "cap:", cap(ch2))
```