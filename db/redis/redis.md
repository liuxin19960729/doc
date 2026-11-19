# redis
## 怎样设计kv
```
怎样设计key
1.单个功能创建就一个key
    取一个有意义的名字
2.相同功能多个key
    :作为分割
例如
    role:10001
    role:10002
    .....     



怎样设计value
1.string(256M)
    
    二进制安全字符串(buffer+length)
        

2.hash
  
  hset
  hget

3.list
  双向循环列表
  插入有序性

  lpush
  lpop
  lrange 


4.set  
  无序集合,唯一


5.zset
    有序集合,唯一







```