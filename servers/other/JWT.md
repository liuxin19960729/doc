# JWT
## 原理
```json
Header.Payload.sinaature


Header:
{
  "alg": "HS256",
  "typ": "JWT"
}

alg:签名算法
typ:命令类型


Payload
携带的数据

Signature
更具Header+Payload 计算值 
用于防止被篡改
```
## 服务器
```
需要准备一个 JWT_SECRET (安全随机秘钥)

为什么Signature可以防止篡改?

header + payload  +  JWT_SECRET 生成签名
JWT_SECRET 保存在服务器 攻击者并不知道 所以被能生成出修改后的签名

```
## JWT 缺点
```
q.退出比较麻烦
JWT 生成好了 只能时间到期退出 没有额外的能直接退出机制

解决方案

解决方案 1 黑名单版本

1.POST  /logout
2.验证JWT 计算出还有多长时间过期
3.JWT 假如Redis 黑名单 并且设置 删除时间 JWT 还有好久过期
4.后续携带这个JWT 会被拒绝访问




解决方案2
会话ID/Token版本

1.通一个账号只能在一台设备上登录
2.新设备登录,旧设备被踢下线
3.点击退出,旧JWT 立即失效
4.管理员封禁账号，或让账号所有设备退出登录


Session ID
每次登录JWT 携带会话ID


数据库(Redis/Mysql)
玩家Id  会话ID



JWT Payload

{
  "sub": "10001", //玩家ID
  "sid": "sess_A1", 会话ID
}



玩家退出 服务器到数据删除 会话Id

JWT 验证
1.是否过期
2.Sid sub 值是否 和数据库里面的值对应
3.不对应拒绝对话


新设备登录 旧设备下线
1. 玩家 10001 登录
2.创建新会话 sess_B
3.将旧会话Sess_标记为失效
4.电脑B正常登录
5.存在webSocket 设备A 断开连接 or 下一次访问接口拒绝


```