# Presence
```js
Presence API 负责进程间通信（IPC）。该 API 提供了发布/订阅（pub/sub）消息机制以及共享的键值存储，所有服务器实例均可访问该存储。



多进程分布式使用下面redis 
npm install --save @colyseus/redis-presence



Room
  this.presence


matchMaker.presence



this.presence.subscribe("global-event", (data) => {
    console.log("received message:", data);
});



//取消所有事件
this.presence.unsubscribe("global-event");



// 取消指定回掉事件
function callback(data) { }
this.presence.subscribe("global-event", callback);
 
// unsubscribing from specific callback
this.presence.unsubscribe("global-event", callback);


、、 发送数据
this.presence.publish("global-event", { any: 1, data: 2, here: "3" });
```

