# router

## RouterSlices
```golang
type RouterSlices struct {
	Apis     map[uint32][]ziface.RouterHandler
	Handlers []ziface.RouterHandler
	sync.RWMutex
}


Apis     map[uint32][]ziface.RouterHandler
一个消息对应者一组处理器


// 全局共享的处理函数
Handlers []ziface.RouterHandler


```

## Use
```golang
func (r *RouterSlices) Use(handles ...ziface.RouterHandler) 


```

## NewRouterSlices
```golang
func NewRouterSlices() *RouterSlices {
	return &RouterSlices{
		Apis:     make(map[uint32][]ziface.RouterHandler, 10),
		Handlers: make([]ziface.RouterHandler, 0, 6),
	}
}



```

## AddHandler
```golang

// 添加handler 并和并前面加的全局 handlers 放入到Apis 中

func (r *RouterSlices) AddHandler(msgId uint32, Handlers ...ziface.RouterHandler) {
	// 1. Check if the API handler method bound to the current msg already exists
	if _, ok := r.Apis[msgId]; ok {
		panic("repeated api , msgId = " + strconv.Itoa(int(msgId)))
	}

	finalSize := len(r.Handlers) + len(Handlers)
	mergedHandlers := make([]ziface.RouterHandler, finalSize)
	copy(mergedHandlers, r.Handlers)
	copy(mergedHandlers[len(r.Handlers):], Handlers)
	r.Apis[msgId] = append(r.Apis[msgId], mergedHandlers...)
}

//1 检查消息是否重复
//2. finalSize=全局handler + addHandler 总长度
// 3.创建 长度为finalSize的切片长度
// 4.将全局handler 拷贝到 mergedHandlers 和 addHandler 拷贝到 mergedHandlers 中
// 5.放入到Apis 中


```

## GetHandlers
```golang



func (r *RouterSlices) GetHandlers(MsgId uint32) ([]ziface.RouterHandler, bool) {
	r.RLock()
	defer r.RUnlock()
	handlers, ok := r.Apis[MsgId]
	return handlers, ok
}

// note: 返回的handlers 引用 而不是拷贝值修改里面的内容会影响路由表

```

## Group
```golang
func (r *RouterSlices) Group(start, end uint32, Handlers ...ziface.RouterHandler) ziface.IGroupRouterSlices {
	return NewGroup(start, end, r, Handlers...)
}
```


## NewGroup
```golang


type GroupRouter struct {
	start    uint32
	end      uint32
	Handlers []ziface.RouterHandler
	router   ziface.IRouterSlices
}

func NewGroup(start, end uint32, router *RouterSlices, Handlers ...ziface.RouterHandler) *GroupRouter {
	g := &GroupRouter{
		start:    start,
		end:      end,
		Handlers: make([]ziface.RouterHandler, 0, len(Handlers)),
		router:   router,
	}
	g.Handlers = append(g.Handlers, Handlers...)
	return g
}

start,end  从start 开始 到 end 结束
handers 共享处理函数




func (g *GroupRouter) Use(Handlers ...ziface.RouterHandler) {
	g.Handlers = append(g.Handlers, Handlers...)
}

func (g *GroupRouter) AddHandler(MsgId uint32, Handlers ...ziface.RouterHandler) {
	if MsgId < g.start || MsgId > g.end {
		panic("add router to group err in msgId:" + strconv.Itoa(int(MsgId)))
	}

	finalSize := len(g.Handlers) + len(Handlers)
	mergedHandlers := make([]ziface.RouterHandler, finalSize)
	copy(mergedHandlers, g.Handlers)
	copy(mergedHandlers[len(g.Handlers):], Handlers)

	g.router.AddHandler(MsgId, mergedHandlers...)
}


// 1.判断消息是否属于当前分组
// 2.finalSize=全局handler + addHandler 总长度
// 3.创建 长度为finalSize的切片长度
// 3.复制处理函数 到切片
// 将msgId 放入router 下的ApIs 转给中




```
