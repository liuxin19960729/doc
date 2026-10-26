# chainbuilder
**znet/chainbuilder.go**
```
责任链模式（Chain of Responsibility）与构造器模式（Builder）的结合

```
## chainbuilder
```golang
// 结构
type chainBuilder struct {
	body       []ziface.IInterceptor
	head, tail ziface.IInterceptor
}

head->body->tail



```

## newChainBuilder
```golang

func newChainBuilder() *chainBuilder {
	return &chainBuilder{
		body: make([]ziface.IInterceptor, 0),
	}
}


创建一个责任链实例

```
## AddInterceptor
```golang
func (ic *chainBuilder) AddInterceptor(interceptor ziface.IInterceptor) {
	ic.body = append(ic.body, interceptor)
}

// 将interceptor 放入到body切片最后面
```

## Execute
```golang
// Execute executes all the interceptors in the current chain in order.
func (ic *chainBuilder) Execute(req ziface.IcReq) ziface.IcResp {

	// Put all the interceptors into the builder
	var interceptors []ziface.IInterceptor
	if ic.head != nil {
		interceptors = append(interceptors, ic.head)
	}
	if len(ic.body) > 0 {
		interceptors = append(interceptors, ic.body...)
	}
	if ic.tail != nil {
		interceptors = append(interceptors, ic.tail)
	}

	// Create a new interceptor chain and execute each interceptor
	chain := zinterceptor.NewChain(interceptors, 0, req)

	// Execute the chain
	return chain.Proceed(req)
}




```
**zinterceptor/chain.go**

## NewChain
```golang



func NewChain(list []ziface.IInterceptor, pos int, req ziface.IcReq) ziface.IChain {
	return &Chain{
		req:          req, //持有当前请求数据
		position:     pos,// 执行到第几个拦截器
		interceptors: list,// 完整的拦截器列表
	}
}

```
## Proceed
```golang
func (c *Chain) Proceed(request ziface.IcReq) ziface.IcResp {
	// 判断是否有拦截器
    if c.position < len(c.interceptors) {
        // 创建下一个chain
		chain := NewChain(c.interceptors, c.position+1, request)

		interceptor := c.interceptors[c.position]
		response := interceptor.Intercept(chain)
		return response
	}
	return request
}


```

**ziface/iinterceptor.go**
## Intercept
```golang
type IInterceptor interface {
	Intercept(IChain) IcResp
	// The interception method of the interceptor (defined by the developer)
	// (拦截器的拦截处理方法,由开发者定义)
}

// Responsibility chain
// (责任链)
type IChain interface {
	Request() IcReq        // Get the request data in the current chain (current interceptor)-获取当前责任链中的请求数据(当前拦截器)
	GetIMessage() IMessage // Get IMessage from Chain (从Chain中获取IMessage)
	Proceed(IcReq) IcResp  // Enter and execute the next interceptor, and pass the request data to the next interceptor (进入并执行下一个拦截器，且将请求数据传递给下一个拦截器)
	ProceedWithIMessage(IMessage, IcReq) IcResp
}


实现 Intercept(IChain) IcResp

   //向下一个intercept 执行 
    
    result:=chain.Proceed(chain.Request())


```


## 总结
```golang
chainBuilder


// head Interceptor 添加
func (ic *chainBuilder) Head(interceptor ziface.IInterceptor)


// tail Interceptor 天机
func (ic *chainBuilder) Tail(interceptor ziface.IInterceptor) 


// body 添加 push 的方式放入到body切片
func (ic *chainBuilder) AddInterceptor(interceptor ziface.IInterceptor) 



func (ic *chainBuilder) Execute(req ziface.IcReq) ziface.IcResp 
     

     interceptors:= [head,body...,tail]

     chain:= zinterceptor.NewChain(zinterceptors,0,req)

    return chain.Proceed(req)




func (c *Chain) Proceed(request ziface.IcReq) ziface.IcResp
   // 判断是否是最后一个interceptor
   // 创建下一个chain
   // 获取当前interptor
   //执行当前interptor(chain)
        intertpor 执行完 返回 chain.Proceed(chain.Request())



```