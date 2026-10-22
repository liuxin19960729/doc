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
[validate](https://github.com/go-playground/validator)
```go
注册自定义验证器

 if v, ok := binding.Validator.Engine().(*validator.Validate); ok {
    v.RegisterValidation("bookabledate", bookableDate)
  }

```
### 仅绑定查询字符串
```go

//http://localhost:8080/bindQuery?aage=29

type Person struct {
	Name string `form:"name"`
	AAge int    `form:"aage"`
}


router.GET("/bindQuery", func(context *gin.Context) {
	var person Person
	if err := context.ShouldBindQuery(&person); err != nil {
		context.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}
	context.JSON(http.StatusOK, person)
})

// 响应
// {"Name":"","AAge":29}
```
### 数据绑定
```

验证规则用在 binding  里面

```
### 绑定查询字符串或 POST 数据
```go
GET  绑定字符串使用  from
POST/PUT 请求 检查 conten-Type 
application/json 使用Json 绑定
application/xml 使用xml 绑定


application/x-www-form-urlencodee
application/form-data
使用表单板顶




type Person struct {
  Name     string    `form:"name"`
  Address  string    `form:"address"`
  Birthday time.Time `form:"birthday" time_format:"2006-01-02" time_utc:"1"`
}


time_format 使用为 年-月-日 time-utc":"1" 解析后的时间为utc

```

### 绑定表单字段的默认值
```go


// 使用默认值
type Person struct {
  Name      string    `form:"name,default=William"`
  Age       int       `form:"age,default=10"`
  Friends   []string  `form:"friends,default=Will;Bill"`                     // multi/csv: use ; in defaults
  Addresses [2]string `form:"addresses,default=foo bar" collection_format:"ssv"`
  LapTimes  []int     `form:"lap_times,default=1;2;3" collection_format:"csv"`
}



collection_format:"ssv"  空格
collection_format:"tsv" 制表符
collection_format:"pipes"  |
collection_format:"multi"  ;
collection_format:"csv"  ;


```
### 数组的集合格式
```go


type Filters struct {
  Tags      []string `form:"tags" collection_format:"csv"`     // /search?tags=go,web,api
  Labels    []string `form:"labels" collection_format:"multi"` // /search?labels=bug&labels=helpwanted
  IdsSSV    []int    `form:"ids_ssv" collection_format:"ssv"`  // /search?ids_ssv=1 2 3
  IdsTSV    []int    `form:"ids_tsv" collection_format:"tsv"`  // /search?ids_tsv=1\t2\t3
  Levels    []int    `form:"levels" collection_format:"pipes"` // /search?levels=1|2|3
}

```
### 绑定URL
```go

// http://localhost:8080/bindUri/lx/987fbc97-4bed-5078-9f07-9141ba07c9f3

type Person2 struct {
	ID   string `uri:"id" binding:"required,uuid"`
	Name string `uri:"name" binding:"required"`
}
// note: ID 添加了一个验证器  uuid
router.GET("/bindUri/:name/:id", func(context *gin.Context) {
	var person Person2
	if err := context.ShouldBindUri(&person); err != nil {
		context.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}
	context.JSON(http.StatusOK, person)
})

```

### 绑定自定义反序列化器
```go
1.满足 encoding.TextUnmarshaler 接口
2.uri/form 标签中指定 parser=encoding.TextUnmarshaler



type Birthday string

func (b *Birthday) UnmarshalText(text []byte) error {
  *b = Birthday(strings.Replace(string(text), "-", "/", -1))
  return nil
}

var _ encoding.TextUnmarshaler = (*Birthday)(nil)




  var request struct {
    Birthday         Birthday   `form:"birthday,parser=encoding.TextUnmarshaler"`
    Birthdays        []Birthday `form:"birthdays,parser=encoding.TextUnmarshaler" collection_format:"csv"`
    BirthdaysDefault []Birthday `form:"birthdaysDef,default=2020-09-01;2020-09-02,parser=encoding.TextUnmarshaler" collection_format:"csv"`
  }




返序列化

type Birthday string

func (b *Birthday) UnmarshalParam(param string) error {
  *b = Birthday(strings.Replace(param, "-", "/", -1))
  return nil
}

var _ binding.BindUnmarshaler = (*Birthday)(nil)




  var request struct {
    Birthday         Birthday   `form:"birthday"`
    Birthdays        []Birthday `form:"birthdays" collection_format:"csv"`
    BirthdaysDefault []Birthday `form:"birthdaysDef,default=2020-09-01;2020-09-02" collection_format:"csv"`
  }




  note: 如果一个类型同时实现了 encoding.TextUnmarshaler 和 BindUnmarshaler，Gin 默认会使用 BindUnmarshaler，除非你在绑定标签中指定了 parser=encoding.TextUnmarshaler

```
### 绑定请求头
```go
ShouldBindHeader 使用 header 标签将http 请求头绑定到结构体中


type HeaderInfo struct {
	AcceptEncoding []string `json:"accept-encoding" header:"accept-encoding" collection_format:"csv"`
}

router.GET("/header", func(context *gin.Context) {
	var header HeaderInfo
	if err := context.ShouldBindHeader(&header); err != nil {
		context.JSON(http.StatusBadRequest, gin.H{"error": err})
		return
	}
	context.JSON(http.StatusOK, header)
})


```

### 绑定 HTML 复选框
```go
type myForm struct {
  Colors []string `form:"colors[]"`
}



c.ShouldBind(&fakeForm)



html 复选框其实就是  
Multipart/Urlencoded post 请求格式

"colors[]=red&colors[]=green&colors[]=blue

```
### Multipart/Urlencoded 绑定
```go
Content-Type   multipart/form-data
Content-Type   application/x-www-form-urlencoded


binding:"required"  表示表单必须填的字段


```

### 使用自定义结构体绑定表单数据请求
```go


type StructA struct {
  FieldA string `form:"field_a"`
}

// 嵌套结构体
type StructB struct {
  NestedStruct StructA
  FieldB       string `form:"field_b"`
}

// 嵌套结构体指针
type StructC struct {
  NestedStructPointer *StructA
  FieldC              string `form:"field_c"`
}

// 嵌套匿名结构体
type StructD struct {
  NestedAnonyStruct struct {
    FieldX string `form:"field_x"`
  }
  FieldD string `form:"field_d"`
}

上面的这三种方式都能使用扁平的惨数进行绑定

```
### 将请求体绑定到不同的结构体
```go
note:不能对同一个请求调用多次获取请求c.Request.Body 的数据 
它是一个 io.ReadCloser——一旦读取后就无法再次读取



c.ShouldBindBodyWith(&objA, binding.JSON)

c.ShouldBindBodyWith 在绑定之前将请求体存储到上下文中(源码中 c.Set(BodyBytesKey, body))所以多次使用请求体绑定不会有问题



```
