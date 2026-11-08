# uuid
```golang
雪花算法

[unused]
1bits
[time in milliseconds]
41bits 精确到毫秒




[data center id]
    5bits  机器实例id
[worker id]
5bits 


[data center id] + [worker id]
10bits  0-1023
   表示不通的机器 上运行不通的任务


[sequence_id]
12bits
    0-4095

单台机器  1ms 最多生成 4096个uuid 才不能重复






sonyflake

39 bit timeStap  + 8 bits sequenceId
单台机器  10ms 最多生成 4096个uuid 才不能重复



sonyflake 表示的时间长度更加长



```