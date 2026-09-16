# zk
[office doc](https://zookeeper.apache.org/doc/r3.5.7/zookeeperStarted.html)
## 集群配置
```
集群模式zoo.cfg 添加配置

server.myid=ip:2888:3888

myid: 表示 zoo.cfg 里面 配置  dataDir 目录里面 myid 文件里面的内容
ip: 表示 myid 服务器的ip
2888:leader 和 fallower 之间的通讯 用于信息交换
3888:leader 服务挂了 用于新leader 的选举


note:
ZooKeeper 集群的可用性遵循严格规则：存活节点数必须 > 总节点数的一半(也就是说服务器存活数量大于总节点数量一半才会对外提供服务)
例如 总共3台服务
    挂掉一台 存活 2台  2>1.5 对外提供服务
    在挂掉一台 存活 1台 1<1.5 拒绝对外提供服务


```
## 第一次启动选举机制
```
假如与 5台机器

1.
Server1
myid=1
LOOKING
1

服务器1启动,投自己一票,判断自己的选票是否大于总机器台数一般(Math.floor(5/2)) ,未足条件状态设置为LOOKING 


2.
Server1
myid=1
LOOKING
0


Server2
myid=2
LOOKING
2

服务器2启动,投自己一票,服务器1和服务2交换信息,服务器1发现服务器2的myid大于自己于是将自己的选票全部给服务2。此时服务器1 0票 服务2 2票 都没有达到大于Math.floor(5/2) 所有服务器1和服务器2的状态都为LOOKING



3.

Server1
myid=1
FOLLOWING
0


Server2
myid=2
FOLLOWING
0

Server3
myid=3
LEADING
3

服务器3启动,投自己一票,然后和服务器1和服务2交换信息,服务器1和服务器2 将自己的票给服务器3,此时服务器3有3票大于Math.floor(5/2) 所以此时的桩状态服务器1和2的状态被设置为FLOOOWING ,服务器3的状态被设置为LEADING


4.


Server1
myid=1
FOLLOWING
0


Server2
myid=2
FOLLOWING
0

Server3
myid=3
LEADING
3

Server4
myid=4
LEADING
1

服务器4启动,投自己一票,和 服务器1,2,3交换信息 发现 服务器1,2,3 已经不是LOOKING 状态，不会更改选票信息,服务器3为3票,服务器4为1,此时服务器4服从多数,并将状态更改为FOLLOWING


5.
Server1
myid=1
FOLLOWING
0


Server2
myid=2
FOLLOWING
0

Server3
myid=3
LEADING
3

Server4
myid=4
LEADING
1

Server5
myid=5
LEADING
1
第5台服务器和第4台服务器一样
```

## 概念
```
ZXID:每次写操作都有一个事务ID
    note:某一时刻 每一台服务ZXID 不一定一致
    ZXID 大代表数据越新
SID:服务ID 每一台服务器编号唯一 和myid 一致

Epoch:每个Leader 任期的代号
    如果此时没有Leader ,此时投票轮数使用Epoch 值 ,每投完一次票Epoch 值就 增加
```
## 非第一启动选举机制
```

Leader 选举的情况
1.服务器初始化的时候
2.服务器运行期间无法和Leader保持连接



当一台服务进入到选举流程的时候
1.当前集群本来就存在一个Leader(只是没有连接上)
    进入选举流程的时候会被告知当前有Leader 并且会将当前Leader 的信息告诉该服务器,当前服务只需要根据告知的Leader 信息并且去连接，连接成功并且数据等状态同步即可
2.当前集群不存在Leader 

Leader 选举规则
  1.Epoch 大的胜出
  2.Epoch 相等 ZXID 大的胜出
  3.Epoch 和 ZXID 相等 ,myid 大的胜出



```