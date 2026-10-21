# config
## server
### setup
#### Transport
```js
客户单和服务器通信

默认使用TCP/Websocket 进行通讯

note: 

    默认使用 @colyseus/ws-transport WebSocketTransport
    

若果要使用自定义则在 里面设置
  defineServer({
    transport:xxxx
  })



```
##### Connection LifeCycle
```
1.Accept 
2.hearbeat (检测无响应的客户端)
3.解析 sessionId roomId 认证token
4.通过matchMake 将连接转发的相应的房间

```

#### Presence
```
房间之间 or 进程见通信方案

```
#### Driver
```js
用于存储和房间查询


const server = defineServer({
    // ...
    presence: new RedisPresence()
});


```

#### Before Listen
```js

服务器启动listen 之前 

const server = defineServer({
    // ...
    beforeListen: async () => {
        //可以连接服务器
        // 可以初始化一些东邪
        .....
    },
});
```

#### Greeting Banner
```js
是否显示欢迎横幅
const server = defineServer({
    greet:true,//default
});

```

#### Select Process ID to Create Room
```js
多进程部署 自定义在那个进程创建房间

默认 选择拥有房间数量最少的进程创建房间



const server = defineServer({
    selectProcessIdToCreateRoom: async function (roomName: string, clientOptions: any) {
        return (await matchMaker.stats.fetchAll())
            .sort((p1, p2) => p1.roomCount > p2.roomCount ? 1 : -1)[0]
            .processId;
    },
});


note:
roomcount 房间数量
ccu  连接数量
```

#### Standalone Matchmaker
```js
const server = defineServer({
    isStandaloneMatchMaker: false,
});


isStandaloneMatchMaker true 该进程值做匹配工作,房间的创建都委托给其他进程

```

#### Development Mode
```js
const server = defineServer({
    devMode: false, //默认false
});
```

#### Graceful Shutdown 优雅关机
#### Method
```js
Define Room Type

// 匹配器会通过房间定义来匹配房间
const server = defineServer({
  rooms: {
    // Define "chat" room
    chat: defineRoom(ChatRoom),
 
    // Define "battle" room
    battle: defineRoom(BattleRoom),
 
    // Define "battle" room with custom options

    battle_woods: defineRoom(BattleRoom, { map: "woods" }),
  }
})


// 房间是更具客户端请求而创建的

// battle_woods: defineRoom(BattleRoom, { map: "woods" }), 
  里面的{map:"woods"} 会和客户端的options合并然后传入到onCreate



```
#### Definition Option
#### Lifecycle Events
```js

通过该方法 在房间外部监听房间的生命周期事件
const server = defineServer({
  rooms: {
    chat: defineRoom(ChatRoom)
      .on("create", (room) => console.log("room created:", room.roomId))
      .on("dispose", (room) => console.log("room disposed:", room.roomId))
      .on("join", (room, client) => console.log(client.sessionId, "joined", room.roomId))
      .on("leave", (room, client) => console.log(client.sessionId, "left", room.roomId))
  }
})



note:
    这操作主要用于日志 和监听房间状态

```

#### Remove Room Type
```js
server.removeRoomType("battle");
撤销该房间 使该房间不能用于匹配
note:
   不建议使用该方法

```

#### Simulate Latency
```js
模拟延迟

server.simulateLatency(200); // 200ms
```

#### Start Listening
#### On Before Shutdown
```js

// 在优雅关机之前被调用
server.onBeforeShutdown(async () => {
    // ... custom logic
});
```
#### On Shutdown
```js
// 在优雅关机完成后被掉调用
server.onShutdown(async () => {
    // ... custom logic
});
```
#### Graceful Shutdown
```js

server.gracefullyShutdown();


1.异常为捕获
2.进程接收到SIGINT,SIGTERM,SIGUSR2程序会自动调用 server.gracefullyShutdown();
```