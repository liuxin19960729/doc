# uWebSockets
```js
如果要使用该协议 则需要安装兼容层
npm install --save uwebsockets-express



const server = defineServer({
  // ...
  transport: new uWebSocketsTransport({
    /* transport options */
  }, {
    /* ssl options */
  }),

});
```
