# gin

## HTTP 方法
```go

router.METHOD


METHOD:

GET	获取资源
POST	创建新资源
PUT	替换现有资源
PATCH	部分更新现有资源
DELETE	删除资源
HEAD	与 GET 相同但不返回响应体
OPTIONS	描述通信选项
```

## 路径参数
```
:name 单个路近段
    /user/:name
    匹配 /user/john
    name 的值就是john


*action 
/user/:name/*action 

/user/john/send 和 /user/john/。捕获的值包含前导 /

通配符 *action 的值始终包含前导 /

```

## 查询字符串参数
```go

// http://localhost:8080/query?name=lx&pwd=123
router.GET("/query", func(context *gin.Context) {
	name := context.Query("name")
	pwd := context.DefaultQuery("pwd", "know")
	context.JSON(http.StatusOK, gin.H{"name": name, "pwd": pwd})
})

```
## 路由
```go

```