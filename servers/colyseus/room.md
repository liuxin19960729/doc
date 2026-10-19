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
```

```