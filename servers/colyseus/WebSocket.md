# WebSocket Transport (via ws)
```js
WebSocket Transport是默认的选项



new WebSocketTransport(options)



```

## Available options
### options server
```js
server
默认 Nodejs 自带的http服务器



express+eample

const app=new express()
const httpserver=createServer(app)


new WebSocketTransport({
    server:httpserver
})

```
### pingInterval
```
default 3000

间隔多长时间 ping 客户端

```
### pingMaxRetries
```
defualt 2
ping 没有响应的最大次数

```
### maxPayload
```
defalult 3kb

客户端发送消息 每条最大数据大小
```

### perMessageDeflate
```
默认 false
开启或关闭 deflate压缩 ,关闭减少cpu的使用
```

###  beforeUpgrade
```
回掉函数在websocket 握手之前被调用
```
### verifyClient
```
verifyClient  为设置 则握手自动同意

websocket-server.js
用于验证客户端是否握手

```

[link](https://nodejs.org/api/cli.html#--max-http-header-sizesize)
```
note: http heaer 最大 16kb 如果要设置大小则
NODE_OPTIONS="--max-http-header-size=32768"

```