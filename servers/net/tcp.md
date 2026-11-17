# tcp
```
四元组:src ip ,dst ip ,src port ,dst port
四元组构成唯一控制协议会话


TCP 握手 建立连接
SYN:
    SEQ:初始化随机一个序列号

    TTL:每次经过一次转发-1
        初始值(14,128,255..)
        note:如果TTL 值为0则会丢弃数据包
    


SYN ACK 
    ACK SEQ  将接受的TCP SYN SEQ+1 返回


ACK
    ACK SEQ  将接受的TCP SYN SEQ+1 返回




TCP
   Header Length 头部长度
   
   TCP 选项只在SYN 包 里面交换信息



窗口实际大小：Window*(window scale )

   Options: 
   1.Window:窗口大小(0XFF 最大65535)
        告诉对方最多一次发送的数据大小
    
   2.window scale    
        note:该选项是后面TCP 新加的选项
        如果要使用该现象的值 则需要看对方是否支持该协议
        
        note:
            如果对方没有返回窗口缩放选项返回则对方不支持

   3.MSS:最大报文段长度


   TCP 选项里面有一些是确认选项
      只有当双方都确认了该选项才能使用该选项
发送数据
例如:
    len=5
    seq=100
    ack 上一次 seq +len

期望下一次ack seq=seq+len


ACK=上一次seq+len
    TCP 就能够删除腾出这len字节的空间



```