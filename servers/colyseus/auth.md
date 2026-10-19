# auth

## anonymous 匿名登录
``` ts
1.
client post /anonymous

2.
服务器相应

{user:{anonymous:true,anonymousId:string}
,token:string}

3.
http.token 和 存入浏览器storage



4.http 每次请求服务器带上该认真的token
header:
    authorization:Bearer token



// 服务器接收到post  /anonymous
anonymousId 构造 

JWT payload:{
    anonymous:true
    anonymousId:string
}


note: nanoid(21) 生成的 anonymousId
      nanoid 并不保证生成出来的不重复 我们可以使用数据库 等存储设备保持已经使用了的id 如果生成出来的在数据中存在则重新生成




note:
如果重新写了 auth.settings.onRegisterAnonymously 旧不使用默认的 


const options = ctx.body?.options;
      const user = auth.settings.onRegisterAnonymously ? await auth.settings.onRegisterAnonymously(options) : { ...options, id: void 0, anonymousId: generateId(21), anonymous: true };




```
## email password 登录
```js
1.
client post /login 
email passowrd 参数


2.服务器 
email 到数据查询 用户信息
3.将传入的password 加密 在和数据库里面的密码比较
4.将用户数据传入到 onCheckBanned(user) 判断是否封禁 
  note:该 方法需要再 auth.settings.onCheckBanned 实现
4.响应数据
  {
    user:{email:string},
    token:string
  }




JWT payload:{
    email
}

```

## register 注册
```ts 
post /register  

1.接收 email  password 值
2. auth.settings.onFindUserByEmail 更具email 查询是否存在该用户
3.存在该用户直接返回用户存在
4.header  authorization 存在 验证  更新 options.upgradingToken
5.auth.settings.onRegisterWithEmailAndPassword(email,hash(password),option)
  note: 注意这里的password 不是存入明文
6.查询 用户 并生成token
7. auth.settings.onSendEmailConfirmation 实现将消息发送邮件服务器 通过邮件服务器确认
    确认地址 url?token=xxxxx

auth.settings.onRegisterWithEmailAndPassword

```

## confirm-email 确认邮件接口
```
get /confirm-email?token=xxx
1.获取参数的token
2.验证token 是否合法
3.auth.settings.onEmailConfirmed(data.email) 确认
4.返回是否正确

auth.settings.onEmailConfirmed
```
## userdata 获取用户信息
```
get /userdata

1.auth.settings.onParseToken(data) 实现
  note: data 是 token 检验后 payload 的数据

```

## forgot-password 忘记密码
```
post /forgot-password

1.必须实现 auth.settings.onForgotPassword
2.通过email 查看是否有该用户
3.生成token 
   payload {email} 和 过期时间 30s

4.auth.settings.onForgotPassword 
    发送邮件 并给重置链接

```

## reset-password 重置密码
```
get /reset-password

1.验证token 否是合法
2.检查token 是否已经使用
3.重写  await auth.settings.onResetPassword 用于 重置密码
5.将老token设置时间 不能无休止的重置
```