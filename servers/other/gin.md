# gin
## 路由
### HTTP 方法
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

### 路径参数
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

### 查询字符串参数
```go

// http://localhost:8080/query?name=lx&pwd=123
router.GET("/query", func(context *gin.Context) {
	name := context.Query("name")
	pwd := context.DefaultQuery("pwd", "know")
	context.JSON(http.StatusOK, gin.H{"name": name, "pwd": pwd})
})

```
### Multipart/Urlencoded 表单
```go

// body 参数  "message=hello&nick=crl"
router.POST("/multipart", func(context *gin.Context) {
	message := context.PostForm("message")
	nikc := context.DefaultPostForm("nick", "know")
	context.JSON(http.StatusOK, gin.H{"nick": nikc, "message": message})
})
```

### 查询字符串和表单
```go

context.PostForm
context.DefaultPostForm

application/x-www-form-urlencoded 或 multipart/form-data



url 参数上的值获取
context.Query
context.DefaultPostForm

curl -d POST http://127.0.0.1:8080/multipart?age=155  --data "message=msg&nick=vv"


```

### Map 作为查询字符串或表单参数
```go

// curl -d POST  "http://127.0.0.1:8080/map?key\[name\]=lx&key\[age\]=30"  --data  "key[name]=crl&key[age]=2


router.POST("/map", func(context *gin.Context) {
	qMap := context.QueryMap("key")
	pMap := context.PostFormMap("key")
	context.JSON(http.StatusOK, gin.H{"qMap": qMap, "pMap": pMap})
})

```
### 路由分配
```golang

// http://localhost:8080/g1/a

{
	group := router.Group("/g1")
	group.GET("/a", func(context *gin.Context) {
		context.String(http.StatusOK, "/g1/a")
	})
}
```

### 重定向
```go

router.GET("/old", func(c *gin.Context) {
	c.Redirect(http.StatusMovedPermanently, "https://www.google.com/")
})


//内部从定向
router.GET("/test", func(c *gin.Context) {
    c.Request.URL.Path = "/final"
    router.HandleContext(c)
  })

note:
 POST 请求重定向重定向使用302 or 307 
```
## 数据绑定
### 模型绑定和验证
```go
类型
Must bind  ->
    Bind BindJson BindXML BindQuery BindYAML
 ->   MustBindWith
  存在绑定错误 c.AbortWithError(400,err).setType(ErrorTyepBind)
  Content-Type 设置 为 text/plain;charsetutf-8


ShouldBind
    ShouldBind、ShouldBindJSON、ShouldBindXML、ShouldBindQuery、ShouldBindYAML
->shouldBindWith
    如果绑定错误 返回 由来开发者适当的处理请求和错误



gin 更具 Content-Type 判断绑定器


type Login struct {
  User     string `form:"user" json:"user" xml:"user"  binding:"required"`
  Password string `form:"password" json:"password" xml:"password" binding:"required"`
}

User 是 bindindg:required
 在绑定的时候值为nil 则将返回错误
 
 
 binding:"-" 不是一个必须传入的值

```
### 自定义验证器
```go

```