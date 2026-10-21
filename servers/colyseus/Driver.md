# Driver
```
负责 匹配器存储和查询的数据

```
## In-Memory Driver
```
Colyseus 默认的driver
将数据存储在内存中 该驱动程序适用于开发和小规模单进程应用程序

```

## Redis Driver
```js
npm install --save @colyseus/redis-driver

用于生产的大规模多进程应用

const server = defineServer({
    driver: new RedisDriver(),
    // ...
});


redis 连接选项 
// Using a connection URL
new RedisDriver("redis://username:password@localhost:6379/0")
 
// Using a port number (connects to localhost)
new RedisDriver(6379)
 
// Using RedisOptions object
new RedisDriver({
    host: "localhost",
    port: 6379,
    password: "your-password",
    db: 0,
    // ... any ioredis options
})


```

## Redis Cluster
## Database Driver
```js
程序启动时更具 Create table if not exists 创建 colyseus_room_caches表


```