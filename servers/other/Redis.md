# Redis
## 入门
### Redis数据类型
```
string
    get key
    set key value
    del key
list (linklist)
    lpush key value
    rpush  key value
    lpop  key
    rpop key
    lindex key index
         获取指定位置元素
    lrange key start end
    获取范围内的元素
    获取所有元素 lrange key 0 -1

set
    可以存入很多个字符串 并且没有字符串去重
    sadd key value
    srem key value  //从集合里面移除元素
    sismember key value 查询value 是否在集合内
    smembers key  获取集合所有元素 
        // note: smembers 执行的命名可能会很慢

hash
    存储的数据既可以是字符串也可以是数字
    hset key subkey value
    hgetall key
    hdel key subkey
    hget key subkey

zzet (有顺序的集合) note:根据score 排序
    zadd key score value
    zrange key start end withscores
    zrem key
    zrangebyscore key min max 
      获取 min<=score<=max 返回的数据
    
```




## 核心概念

### 字符串
```
value 类型
    字符串
    整数
    浮点数


value 浮点数 or 整数 
    incr  keyamount
    decr key amount 
    incrby key amount 
    decrby key amout
    
    // >= v.2.6
    increbyfloat key amount


note:
   incr or decr
   incrby or decrby
   INCRBYFLOAT
    一个不存在的key 的时候 会将该值默认当成0值操作



value 字符串
appens key value 
    追加字符串
getrange key start end
    相等于 substr

setrang key offset value
    设置指定位置的值
    流入 原始字符串 "sssll"

    setrange key 1 “222”
    则变成字符串 "s222"

getbit key offset
    返回 offset bit

    例如 
    set k "A"
    A ascii 二进制为 01000001

    note: offset 计算是从左到右

    getbit k 0; //0
    getbit k 1; 1
    
 sebit key offset value
    


bitcount key start end [BIT|BYTE(default)]
    统计二进制位字符串里面为1的二进制数量


bitop op dstkey key1,....
    op: and or xor not 
    dstkey 存储的结果
    key1,.... 参与运算的key
```