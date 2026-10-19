# auth

## anonymous 匿名登录
``` 
1.
client post /anonymous

2.
服务器相应

{user:{anonymous:true},token:string}

3.
http.token 和 存入浏览器storage



4.http 每次请求服务器带上该认真的token
header:
    authorization:Bearer token




JWT payload:{
    anonymous:true
}
```