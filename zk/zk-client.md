# zk-client

```
zkCli.sh -server xxxx:2181 指定连接



ls -s path 节点详细信息


获取值 
get -s  path 
get path 

修改节点的值
set path value


查看节点的状态
stat path


czxid 创建节点的事务id
ctime 创建节点的时间
mzxid 最后一次修改值的事务id
mtime 最后一次修改值的时间
pzxid 子节点的事务id

cversion 版本号
dataVersion 
aclVersion

ephemeralOwner 是否是临时节点
dataLength 数据的长度
numChildren 子节点个数


```

## 节点
```
节点类型:
持久节点:服务端与客户端断开连接节点不删除


短暂节点:服务端与客户端断开连接节点删除
创建持久节点
create path value 
create -s  path value  创建一个带序号的节点(程序会自动在后面加上序号)
例如

create -s /sanguo/names zhangfei
// output
Created /sanguo/names0000000001

create -s /sanguo/names guanyu
// output
Created /sanguo/names0000000002

创建临时节点
crate -e path value
创建临时带序号的节点
create -e -s path value



```