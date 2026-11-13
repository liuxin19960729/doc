# nginx
## install
```
//https
sudo apt install libssl-dev

//数据压缩模块
sudo apt install zlib1g-dev

// 正则表达式
sudo apt install libpcre2-dev


nginx-1.22.1.tar.gz 


// 配置

./configure --prefix=/home/liuxin/bin/nginx --with-http_realip_module --with-http_addition_module --with-http_gzip_static_module --with-http_secure_link_module --with-http_stub_status_module --with-stream 


//编译 安装
make && make install



// 查看nginx 进程
ps -aux | grep nginx



所有请求的处理都worker进程处理

1.怎么样做到master 创建listenfd ,worker进程处理监听的?
   1.master 进程 进行 listenfd 创建 和listen(listenfd)
   2.master 进程 fork()
     1.子进程 继承listenfd 继续处理
     2.父进程 关闭listenfd 继续处理



2.多个worker进程都在监听listenfd,连接来了谁来accept()?
    当一个连接来的时候多个进程epoll_await() 都返回 出现惊群现象

    nginx 在同一时刻将listenfd 假如epoll 判断是否可以accept

```

## conf
[load_balancing](https://nginx.org/en/docs/http/load_balancing.html)
```


# worker进程数量 最好和cpu数量一样
worker_processes 8;

同时监听多个端口

http {

    server {
        listen 9999;
    }

    server {
        listen 8888;
    }

    server {
        listen 7777;
    }

    server {
        listen 6666;
    }
}


netstat -ap | grep nginx

tcp        0      0 0.0.0.0:8888            0.0.0.0:*               LISTEN      29764/nginx: master 
tcp        0      0 0.0.0.0:9999            0.0.0.0:*               LISTEN      29764/nginx: master 
tcp        0      0 0.0.0.0:6666            0.0.0.0:*               LISTEN      29764/nginx: master 
tcp        0      0 0.0.0.0:7777            0.0.0.0:*               LISTEN      29764/nginx: master 



// 设置反向代理
    server {
        listen 9999;
        location / {
             proxy_pass http://www.baidu.com;
        }
       
    }



// 同一个地址反向代理多个服务器
    upstream backend {
        server 127.0.0.1:8888;
        server 127.0.0.1:7777;
    }
    server {
        listen 9999;
        location / {
             proxy_pass http://backend;
        }
       
    }


```

## nginx 数据结构使用
```c
测试环境:
makefile

CC=gcc
nginx_code:	nginx_code.c
	$(CC)  -I/home/liuxin/nginx-1.22.1/src/core -I/home/liuxin/nginx-1.22.1/src/os/unix -I/home/liuxin/nginx-1.22.1/objs -I/home/liuxin/nginx-1.22.1/src/event  $^   -o $@ 





// nginx 数组

typedef struct {
    void        *elts;// 指向数组的起始位置
    ngx_uint_t   nelts;// 已经使用slot 个数
    size_t       size;// 每个元素的大小
    ngx_uint_t   nalloc;//有多少个元素
    ngx_pool_t  *pool;
} ngx_array_t;


```