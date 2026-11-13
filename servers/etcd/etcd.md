# etcd
```
共享配置 服务发现 集群监控 leader选举 分布式锁...
```
## V2和v3版本的比较
```
通信方式
v2 http+json
v3 grpc+protobuf


v3 使用 lease(租约) 实现过期机制
v3 支持事务支持多版本并发的磁盘数据库
v3 扁平的kv结构


```
## etcd 架构
```

server<-grpc-> server
    flollower<-grpc->leader 
client-grpc->server


boltdb 是单机支持事务的kv存储



wal(write ahead log) 预写日志
    执行写操作前先写预写日志

snapshot 快照
   用于其他节点同步主节点数据而达成一致性地状态



```

## etcd 操作
### kv 操作
```

// 设置返回上一次key 对应的值
./etcdctl put key lx --prev-kv 

// 返回 key 前缀的所有值
./etcdctl get key  --prefix

// 给key绑定租约
./etcdctl put key value --lease=id

// 删除 (note:删除成功返回1)
./etcdctl del key  

// 获取指定版本的key值
./etcdctl get key --rev 

// 获取key 的信息
./etcdctl  get key -w json

```

### get
```
// key 开头的名字信息
key --prefix



etcdctl get key key3
     [key,key3)  范围捏
     note: 比较是通过字符串比较


etcdctl get key --from-key
      >=key


// 只返回key
etcdctl get key --key-only



--limit 限制返回的数量


--sort-by=value 以值排序返回 create 创建时间 modify修改时间  version 版本 key 


--order ASCEND 升序(默认)
        DESCEND 降序


--rev=xx 
    获取以前的版本信息
```


### lease 租约 
```
创建租约
./etcdctl lease grant 100
返回租约id

释放租约
./etcdctl lease revoke id

查看租约信息
./etcdctl lease timetolive id
租约续约
./etcdctl lease keep-alive id

```
### watch
```
// 监听key 的变动
etcdctl wath key

// 监听返回上一个key 对应的值
./etcdctl watch key  --prev-kv


// 监听以key 为前缀的所有key的变动
./etcdctl watch --prefix key 





```
### 事务
```json
etcd 版本控制

./etcdctl  get key -w json



{"header":{"cluster_id":14841639068965178418,"member_id":10276657743932975437,"revision":38,"raft_term":2},"kvs":[{"key":"a2V5","create_revision":38,"mod_revision":38,"version":1,"value":"Y3Js"}],"count":1}


cluster_id 集群id
member_id 节点id
revision 版本id  
    etcd 每一修改版本都会+1
    增 删 该


create_revision 创建key的时候全局revision的版本号
mod_revision 修改key对应的值 全局revision的版本号


raft_term 选举任期id
version 被修改的次数



./etcdctl tnx -i

// 比较
value("key")=="other"


// 成功操作
可以设置多个命令


// 失败操作
可以设置多个命令


value("key") 获取key对应的值
mod("key“) 修改版本信息
create("key") 创建版本
version("key") 修改了几次

```