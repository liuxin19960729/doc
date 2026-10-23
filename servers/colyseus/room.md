# room
```
房间接口设计

服务器
POST
"/matchmake/:method/:roomName"

客户端可以通过不同的路径匹配到对应的函数实现

joinOrCreate
create
join
joinById


客户端会将options的json 通过http body 传入 
服务器直接通过 clientOptions 去 获取就可以了


 //客户端IP获得
    firstHop("x-real-ip") ?? firstHop("x-forwarded-for") ?? firstHop("x-client-ip") ?? (remoteAddress || void 0);
```

## create
```js
1.callOnAuth(roomName,clientOptions,authContext)
   
   onAuth() 如果返回false null undefine 则 认证失败
   
   返回非 true 则 返回对应的值

2.createRoom(roomName, clientOptions)
    // 选取roomCount最小的返回
   async selectProcessIdToCreateRoom() 
    async function fetchAll() Stats.mjs 
    //获取所有的房间数据 Array<{processId,roomCount,ccu}>


   handleCreateRoom(roomName, clientOptions, restoringRoomId, presetRoomId) 
       room:
             __init()





note:
    默认 Room类里面框架帮我们实现了onAuth 来认证
    如果要扩展和自定义 需要 在基类里面  static onAuth() 来覆盖
    框架的代码


    onAuth 返回false 拒绝创建房间
    onAuth 返回非 null 值则将返回的值传入 onJoin里面



```







## room Lifecycle Events
### On Create
```js


onCreate (options) {
    /**
     * This is a good place to initialize your room state.
     */
}

defineRoom(Class,options) 服务器通过options 提供选项 

note:
     客户端 + 服务端选项  onCreate (options)  
     服务器选项可能覆盖客户端选项
     
```
### OnAuth
```
onJoin() 之前调用
```
### OnJoin
```
onAuth 认证成功 进入房间成功 调用功能



client sdk .joinOrCreate  or .join
```

### On Drop
```
当客户端非主动断结连接的情况下可以在onDrop 触发重新连接机制

```
### onReconnect
```
客户端重连成功后调用
```
### On Leave
```
onDrop
   客户端断 .leave()

    or 断线连接失败调用onLeave

```

### On Dispose
```
房间销毁该回掉触发

客户端全部离开房间 autoDispose被设置true
```

### On Unhandled Exception
```
在任何一个声明周期函数触发异常被捕获都会触发该方法

```

### On Before Patch
```
在状态同步之前触发
```
### On Before Shutdown
```
在优雅关闭的过程中被调用

默认情况下，聊天室会断开所有客户端的连接并立即释放自身。

您可以自定义聊天室在关闭过程中的行为：
```