# async op

## async_op.go
```golang


func Process(opId int, asyncOp func())
    更具opId 获取对应的worker 执行异步操作


func getCurWorker(opId int) *AsyncWorker
    workerIndex := opId % workerCount
	curWorker := asyncWorkerArray[workerIndex]

    if curWorker != nil {
        return curWorker
	}


    // 创建worker
    // note: taskQ = make(chan func(), 2048) 2048 通道2048个缓存
    // 运行worker
    // 返回worker
    return curWorker


```

## async_worker.go
```golang


type AsyncWorker struct {
	taskQ chan func()
}


func (aw *AsyncWorker) process(asyncOp func())
    aw.taskQ <- func() {
		defer func (){
            // 捕获aysncOP 的Panic 
            if err:=recover();err!=nil{

            }
        }()

        // 执行传入过来的任务
        asyncOp()
	}

func (aw *AsyncWorker) loopExecTask()

    for{
        // 阻塞到有任务过来的执行对应的任务
        task:=<-aw.taskQ
		
        if task!=nil{
			task()
		}
    }

```

## async_op_result.go

```golang


type AsyncOpResult struct {
	// Player connection (玩家链接)
	conn ziface.IConnection
	//Returned object (已返回对象)
	returnedObj interface{}
	// Completion callback function(完成回调函数)
	completeFunc func()
	// Whether the return value has been set(是否已有返回值)
	hasReturnedObj int32
	// Whether the completion callback function has been set(是否已有回调函数)
	hasCompleteFunc int32
	// Whether the completion function has already been called(是否已经调用过完成函数)
	completeFuncHasAlreadyBeenCalled int32 // Default value = 0, not yet called (默认值 = 0, 没被调用过)
}


保存一步结果
// SetReturnedObj sets the return value(设置返回值)
func (aor *AsyncOpResult) SetReturnedObj(val interface{}) {
	if atomic.CompareAndSwapInt32(&aor.hasReturnedObj, 0, 1) {
		aor.returnedObj = val
		// **** 防止未调用回调函数问题: 设置处理结果时，直接调用回调函数：1.回调函数未绑定，调用空；2.回调函数已绑定,立马调用 ****
		// **** Prevent the problem of not calling the completion callback function:
		// Call the callback function directly when setting the processing result:
		// 1. If the callback function is not bound, it will be called with nil;
		// 2. If the callback function is bound, it will be called immediately. ****
		aor.doComplete()
	}
}

保存一步任务结果，尝试触发完成任务



注册完成回掉

// OnComplete sets the completion callback function(完成回调函数)
func (aor *AsyncOpResult) OnComplete(val func()) {
	if atomic.CompareAndSwapInt32(&aor.hasCompleteFunc, 0, 1) {
		aor.completeFunc = val

		// **** 防止未调用回调函数问题:设置回调函数时，发现已经有处理结果了，直接调用 ****
		// **** Prevent the problem of not calling the completion callback function:
		// If a processing result already exists when setting the callback function,
		// call the callback function directly. ****
		if aor.hasReturnedObj == 1 {
			aor.doComplete()
		}
	}
}


// doComplete executes the completion callback function, double-checking when setting the processing result and callback function to prevent the possibility of not calling the callback function
// (执行完成回调,在设置处理结果和回调函数的时候双重检查，杜绝未调用回调函数的可能性)
func (aor *AsyncOpResult) doComplete() {
	if aor.completeFunc == nil {
		return
	}

	// Prevent re-entry problems (防止可重入问题)
	if atomic.CompareAndSwapInt32(&aor.completeFuncHasAlreadyBeenCalled, 0, 1) {
		// Prevent cross-thread calling problems,
		// throw it to the corresponding business thread to execute
		// (防止跨线程调用问题,扔到所属业务线程里去执行)
		request := znet.NewFuncRequest(aor.conn, aor.completeFunc)
		aor.conn.GetMsgHandler().SendMsgToTaskQueue(request)
	}
}




msghandler.go
// SendMsgToTaskQueue sends the message to the TaskQueue for processing by the worker
// (将消息交给TaskQueue,由worker进行处理)
func (mh *MsgHandle) SendMsgToTaskQueue(request ziface.IRequest) {
	workerID := request.GetConnection().GetWorkerID()
	// zlog.Ins().DebugF("Add ConnID=%d request msgID=%d to workerID=%d", request.GetConnection().GetConnID(), request.GetMsgID(), workerID)
	// Send the request message to the task queue
	mh.TaskQueue[workerID] <- request
	zlog.Ins().DebugF("SendMsgToTaskQueue-->%s", hex.EncodeToString(request.GetData()))
}


```

## 怎么使用
```golang
asyncResult := zasync_op.NewAsyncOpResult(conn)

//OpId 选择Worker 执行
zasync_op.Process(1001, func() {
    // 1. 在 Worker goroutine 中执行耗时操作
    user := queryUserFromDB()

    // 2. 保存结果
    asyncResult.SetReturnedObj(user)
})

// 3. 注册完成回调
asyncResult.OnComplete(func() {
    // 4. 在框架安排的业务处理上下文中继续处理
    user := asyncResult.GetReturnedObj()

    initPlayer(user)
    sendLoginResponse(user)
})



执行流程


asyncResult.OnComplete// 注册完成回调
Process func
    getCurWorker
       存在直接返回
       没有创建worker
            	go asyncWorkerArray[workerIndex].loopExecTask()

    curWorker.process(asyncOp)
    task-<func(){aysncOp}
    
  

   loopExecTask 

	for(){
        task:=<-aw.taskQ
	    task()
    }



    task 执行到这一个步骤 asyncResult.SetReturnedObj(user)


    aor.doComplete() 函数执行

    会将你注册completeFunc 扔到 msgHandle里面的worker 执行
    

```