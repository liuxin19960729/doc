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
    
```