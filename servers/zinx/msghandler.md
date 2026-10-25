# msghandler
## 消息处理流程
```
1.TCP 网络层 解析客户端数据 Request {len+id+data}
2.通过Request 数据 Id 找到对应Router 处理器
3.将Router分配给Worker执行
4.Router 处理具体的业务
```
## MsgHandle 结构
```golang
// MsgHandle is the module for handling message processing callbacks
// (对消息的处理回调模块)
type MsgHandle struct {
	// A map property that stores the processing methods for each MsgID
	// (存放每个MsgID 所对应的处理方法的map属性)
	Apis map[uint32]ziface.IRouter

	// The number of worker goroutines in the business work Worker pool
	// (业务工作Worker池的数量)
	WorkerPoolSize uint32

	// A collection of idle workers, used for zconf.WorkerModeBind
	// 空闲worker集合，用于zconf.WorkerModeBind
	freeWorkers  map[uint32]struct{}
	freeWorkerMu sync.Mutex

	// A message queue for workers to take tasks
	// (Worker负责取任务的消息队列)
	TaskQueue []chan ziface.IRequest

	// A collection of extra workers, used for zconf.WorkerModeDynamicBind
	// (池里的工作线程不够用的时候, 可临时额外分配workerID集合, 用于zconf.WorkerModeDynamicBind)
	extraFreeWorkers  map[uint32]struct{}
	extraFreeWorkerMu sync.Mutex

	// Chain builder for the responsibility chain
	// (责任链构造器)
	builder      *chainBuilder
	RouterSlices *RouterSlices
}

```

## newMsgHandle
```golang
WorkerModeBind  // "Bind"

conf.GlobalObject.WorkerPoolSize =  uint32(zconf.GlobalObject.MaxConn)

freeWorkers // 将所有Id workerId 设置为空闲(这里的大小是最大连接数)









WorkerModeDynamicBind // "DynamicBind"
   freeWorkers 值初始化 池大小个空闲Worker
   TaskQueueLen = uint32(zconf.GlobalObject.MaxConn)


// 初始化队列数量
handle.TaskQueue = make([]chan ziface.IRequest, TaskQueueLen)



```

## useWorker
```golang


核心逻辑
mh.freeWorkerMu.Lock()
defer mh.freeWorkerMu.Unlock()

for k := range mh.freeWorkers {
	delete(mh.freeWorkers, k)
	return k
}

// 将freeWorkers k 删除 返回k(Worker ID)表示该k被分配
// note: 这里的k并不是最小的Id因为Map 是无序的



// 动态绑定模式下
1.先尝试从空闲工作池中取一个Id
2.空闲工作池用完了 到额外工作池中取一个Id
   创建chan 
   go StartOneWorker


```
## freeWorker
```golang
释放回收worker

WorkerModeBind //普通绑定模式
    // 将workerId 放入空闲集合里面
    mh.freeWorkers[conn.GetWorkerID()] = struct{}{}


WorkerModeDynamicBind //动态绑定模式
    if workerID < mh.WorkerPoolSize{
        // 将workerId 放入空闲集合里面
        mh.freeWorkers[workerID] = struct{}{}
    }else{
        // 释放worker
        // 将 workerID 放入到额外的空闲集合里面
    }

```
## StopOneWorker
```golang
//停止 channel 
close(mh.TaskQueue[workerID])
```

## Intercept
```golang


iRequest, ok := request.(ziface.IRequest)

if mh.WorkerPoolSize > 0 {
    // 启动了worker池 交给worker 处理
    mh.SendMsgToTaskQueue(iRequest) 
    note:
        将消息写入通道 如果 消息未满直接写入
        如果消息满了 阻塞 说明消费的速度跟不上生产的速度

}else{

    if 非router slice{
         go mh.doMsgHandler(iRequest, WorkerIDWithoutWorkerPool)
    }else{
        go mh.doMsgHandlerSlices(iRequest, WorkerIDWithoutWorkerPool)
    }
   

}



// 非责任链方式执行router
doMsgHandler:
  1.defer 捕获异常
  2.通过msgId 找到对应router handler
  3.request.BindRouter(handler) 
    当前请求与对应的业务处理器管理起来
  4.request Call 执行对应的业务
  4.putRequest(request) 回收request 将request 放回池中




```

## StartOneWorker
```golang
switch req := request.(type) {
case ziface.IFuncRequest:
    mh.doFuncHandler(req, workerID)

case ziface.IRequest:
    // 普通客户端消息处理
}




worker 执行的不仅可以执行客户端请求 还可以执行内部函数调用请求


```
