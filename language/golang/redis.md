# redis
[doc](https://pkg.go.dev/github.com/gomodule/redigo/redis#hdr-Concurrency)
```golang
go get github.com/gomodule/redigo/redis


// redis 连接设置超时设置
	conn, err := redis.Dial("tcp", "114.55.247.8:6379",
		redis.DialConnectTimeout(time.Duration(1)*time.Second))



// 切换Redis 数据库
redis.DialDatabase(1)




// 执行命令
conn.Do .....

// 获取类型的多个值
redis.Types....
// 获取类型的单个值
redis.Type......



// redis 连接池
redis.Pool
MaxIdle 最大待机连接数
MaxActive 最大连接数
IdleTimeout 空闲等待时间
    超过这个时间 空闲连接将被关闭,如果设置为0不会被关闭,应该设置比服务端超时更加短的时间



// 连接池的使用
	pool := &redis.Pool{
		Dial: func() (redis.Conn, error) {
			return redis.Dial("tcp", address,
				redis.DialPassword(password),
				redis.DialConnectTimeout(time.Duration(1)*time.Second),
				redis.DialDatabase(0),
			)
		},
	}

	defer pool.Close()

	conn := pool.Get()

    // 检查连接是否可用
    if conn.Err() != nil {
		log.Fatal(conn.Err())
	}
    // 处理完关闭该连接
	defer conn.Close()
	conn.Do("set", "name", "lx", "ex", 10)

	name, _ := redis.String(conn.Do("get", "name"))

	fmt.Println(name)


// 获取 连接数等参数统计
pool.Stats()




```