# Redis
## 字符串
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
## 列表
```
rpush key value [value ....]
lpush key value [value ....]
lpop key 
rpop key
lrange key start end
ltrim key start end
   裁剪列表


blpop key1 [key2 ...] timeout
brpop key [key2 ...] timeout


note: 
k 从左到右只要有 key 对应的列表有数据 则 会lpop or rpop 如果都没有
则会阻塞 如果timout 超时直接范湖 或者 等待到k对应的列表里面有数据



rpoplpush source-key dst-key
    将source-key rpop 一个元素  lpush 到 dst-leu
brpoplpush source-keu dest-leu timeout
```

## 集合
```
sadd key item [item ....]
  返回添加了多少个元素

srem key item [item ...]
  返回删除元素个数


sembers key 
    返回集合里面的所有元素



sismember key value
   检测集合里面是否包含某个元素

scard key
  返回集合包含元素的数量
srandmember key [count]
    返回随机元素个数
    note:
        count >=0 不会出现重复的随机
        count < 会出现重复的数

spop key
    随机的删除 一个元素
smove source-key dest-key item
    如果 source-key 对应的集合里面有item 
    则source-key itme 元素移动
    到 dest-key
    并返回 1 否则返回 0


sdiff key [key ....]
    返回那些存在第一个集合 而不存在其他集合
    例如
    uset[ 1,2,3,4]
    uset2[3,4,5,6]
    sdiff uset uset2
    返回[2,3]
   
    例如 uset3[1,2,7,8]
    sdiff uset uset2 uset3
    返回 []


sdiffstore dest-key key [key ...]
    返回那些存在第一个集合 而不存在其他集合 将值存入到 dest-key

sinter key [key....]
   返回存在所有集合的元素
   note: 返回每个集合都存在的元素

singerstore dest-key key [key....]



sunion key [key ....]
返回那些至少存在于一个集合的元素
    uset[1]
    uset2[2]
    sunion uset uset2 返回 [1,2]

sunion dest-key key [key.....]

```

## hash
```
hmget key-name key [key....]
  获取一个值or 多个值

hmset key-name  key value [key value ...]
 设置一个值 or 多个值

hdel key-name key [key....]
   删除 has  里面的key

hlen key-name
    hash 里面有多少个key


hexists key-name key 
    是否存在key

hkeys key-name
   hash 所有 key
hvals key-name
    hash 所有值
hgetall key-name
   获取所有k v

hincrby key-name key increment
 给hash  里面值加上increment

hincrbyfloat key-name key incement

```
## sort set
```
zadd key-name score member [score member ...]
zrem key-name member [member ...]
 从set 里面删除元素

zcard key-name
zincrby
```


## 发布和订阅
```

```