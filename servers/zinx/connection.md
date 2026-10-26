# connection
```golang

type Connection struct {
	// The ID of the current connection, also known as SessionID, globally unique, used by server Connection
	// uint64 range: 0~18,446,744,073,709,551,615
	// This is the maximum number of connID theoretically supported by the process
	// (当前连接的ID 也可以称作为SessionID，ID全局唯一 ，服务端Connection使用
	// uint64 取值范围：0 ~ 18,446,744,073,709,551,615
	// 这个是理论支持的进程connID的最大数量)
	connID uint64


    // The workerid responsible for handling the link
	// 负责处理该连接的workerid
	workerID uint32

    // The message management module that manages MsgID and the corresponding processing method
	// (消息管理MsgID和对应处理方法的消息管理模块)
	msgHandler ziface.IMsgHandle

    // Channel to notify that the connection has exited/stopped
	// (告知该连接已经退出/停止的channel)
	ctx    context.Context
	cancel context.CancelFunc

    	// Buffered channel used for message communication between the read and write goroutines
	// (有缓冲管道，用于读、写两个goroutine之间的消息通信)
	msgBuffChan chan []byte

    	// Go StartWriter Flag
	// (开始初始化写协程标志)
	startWriterFlag int32
}



connID 可以作为SessionID 使用
workerID 分配给Worker 处理连接

msgHandler 消息处理模块
     接收到消息 c.msgHandler.Execute(req) 过滤器->路由->处理对应的消息





ctx    context.Context
cancel context.CancelFunc
    
<-c.ctx.Done()
c.ctx.cancel()
  



msgBuffChan
    发送消息的缓冲队列


startWriterFlag
    是否启动写协程的标志 防止重复启动洗的goroutine
```

###  连接属性与回调
```golang
	// Connection properties
	// (连接属性)
	property map[string]interface{}

	// Lock to protect the current property
	// (保护当前property的锁)
	propertyLock sync.Mutex

	// Which Connection Manager the current connection belongs to
	// (当前连接是属于哪个Connection Manager的)
	connManager ziface.IConnManager

	// Hook function when the current connection is created
	// (当前连接创建时Hook函数)
	onConnStart func(conn ziface.IConnection)

	// Hook function when the current connection is disconnected
	// (当前连接断开时的Hook函数)
	onConnStop func(conn ziface.IConnection)




property  连接级别的属性定义 例如 玩家Id 会话信息 等
  SetProperty
  GetProperty
  RemoveProperty

onConnStart
onConnStop
连接启动 or 关闭回调函数 


connManager :管理当前连接

```

### 协议、心跳与地址
```golang
	// Data packet packaging method
	// (数据报文封包方式)
	packet ziface.IDataPack

	// Last activity time
	// (最后一次活动时间)
	lastActivityTime time.Time

	// Framedecoder for solving fragmentation and packet sticking problems
	// (断粘包解码器)
	frameDecoder ziface.IFrameDecoder

	// Heartbeat checker
	// (心跳检测器)
	hc ziface.IHeartbeatChecker
    // Connection name, default to be the same as the name of the Server/Client that created the connection
	// (连接名称，默认与创建连接的Server/Client的Name一致)
	name string

	// Local address of the current connection
	// (当前连接的本地地址)
	localAddr string

	// Remote address of the current connection
	// (当前连接的远程地址)
	remoteAddr string


TCP 只提供字节流,不保证一次read就能对应客户端发送的一条完整的 消息所以必须考虑消息边界问题



```
## newServerConn
```golang
服务端连接创建


func newServerConn(server ziface.IServer, conn net.Conn, connID uint64) ziface.IConnection {

	// Initialize Conn properties
	c := &Connection{
		conn:            conn,
		bufWriter:       bufio.NewWriterSize(conn, 16*1024),
		connID:          connID,
		connIdStr:       strconv.FormatUint(connID, 10),
		startWriterFlag: 0,
		msgBuffChan:     nil,
		property:        nil,
		name:            server.ServerName(),
		localAddr:       conn.LocalAddr().String(),
		remoteAddr:      conn.RemoteAddr().String(),
	}

	lengthField := server.GetLengthField()
	if lengthField != nil {
		c.frameDecoder = zinterceptor.NewFrameDecoder(*lengthField)
	}

	// Inherited properties from server (从server继承过来的属性)
	c.packet = server.GetPacket()
	c.onConnStart = server.GetOnConnStart()
	c.onConnStop = server.GetOnConnStop()
	c.msgHandler = server.GetMsgHandler()

	// Bind the current Connection with the Server's ConnManager
	// (将当前的Connection与Server的ConnManager绑定)
	c.connManager = server.GetConnMgr()

	// Add the newly created Conn to the connection manager
	// (将新创建的Conn添加到连接管理中)
	server.GetConnMgr().Add(c)

	return c
}


//  server.GetConnMgr().Add(c) 创建连接并且加入到连接管理器中


bufWriter:       bufio.NewWriterSize(conn, 16*1024)
创建一个有16kb 的写缓冲器




```

## Start
```golang

// Start starts the connection and makes the current connection work.
// (启动连接，让当前连接开始工作)
func (c *Connection) Start() {
	defer func() {/
		if err := recover(); err != nil {
			zlog.Ins().ErrorF("Connection Start() error: %v", err)
		}
	}()
	c.ctx, c.cancel = context.WithCancel(context.Background())

	// Execute the hook method for processing business logic when creating a connection
	// (按照用户传递进来的创建连接时需要处理的业务，执行钩子方法)
	c.callOnConnStart()

	// Start heartbeating detection
	if c.hc != nil {// 启动心跳检测
		c.hc.Start()
		c.updateActivity()
	}

	// 占用workerid 分配workerId or 创建并分配workerId
	c.workerID = useWorker(c)

	// Start the Goroutine for reading data from the client
	// (开启用户从客户端读取数据流程的Goroutine)
	go c.StartReader()

	// 直接退出，让 StartReader协程 来处理关闭逻辑

}



c.ctx, c.cancel = context.WithCancel(context.Background())

StartReader 
StartWriter 
这两个 goroutine 可以检测到停止的信号


```

## StartReader
```golang
// StartReader is a goroutine that reads data from the client
// (读消息Goroutine，用于从客户端中读取数据)
func (c *Connection) StartReader() {
	zlog.Ins().InfoF("[Reader Goroutine is running]")
	defer zlog.Ins().InfoF("%s [conn Reader exit!]", c.RemoteAddr().String())
	defer func() {
		c.Stop()
		c.doClose()
	}()
	defer func() {
		if err := recover(); err != nil {
			zlog.Ins().ErrorF("connID=%d, panic err=%v", c.GetConnID(), err)
		}
	}()

	//Reduce buffer allocation times to improve efficiency
	// add by ray 2023-02-03
	buffer := make([]byte, zconf.GlobalObject.IOReadBuffSize)

	for {
		select {
		case <-c.ctx.Done():
			return
		default:

			// read data from the connection's IO into the memory buffer
			// (从conn的IO中读取数据到内存缓冲buffer中)
			n, err := c.conn.Read(buffer)
			if err != nil {
				zlog.Ins().ErrorF("read msg head [read datalen=%d], error = %s", n, err) // 不需要发 Log,正常关闭
				return
			}
			zlog.Ins().DebugF("read buffer %s \n", hex.EncodeToString(buffer[0:n]))

			// If normal data is read from the peer, update the heartbeat detection Active state
			// (正常读取到对端数据，更新心跳检测Active状态)
			if n > 0 && c.hc != nil {
				c.updateActivity()
			}

			// Deal with the custom protocol fragmentation problem, added by uuxia 2023-03-21
			// (处理自定义协议断粘包问题)
			if c.frameDecoder != nil {
				// Decode the 0-n bytes of data read
				// (为读取到的0-n个字节的数据进行解码)
				bufArrays := c.frameDecoder.Decode(buffer[0:n])
				if bufArrays == nil {
					continue
				}
				for _, bytes := range bufArrays {
					// zlog.Ins().DebugF("read buffer %s \n", hex.EncodeToString(bytes))
					msg := zpack.NewMessage(uint32(len(bytes)), bytes)
					// Get the current client's Request data
					// (得到当前客户端请求的Request数据)
					req := GetRequest(c, msg)
					c.msgHandler.Execute(req)
				}
			} else {
				msg := zpack.NewMessage(uint32(n), buffer[0:n])
				// Get the current client's Request data
				// (得到当前客户端请求的Request数据)
				req := GetRequest(c, msg)
				c.msgHandler.Execute(req)
			}
		}
	}
}



buffer := make([]byte, zconf.GlobalObject.IOReadBuffSize)
for 循环外面单独创建 一个buf 防止重复创建 见扫垃圾回收


case <-c.ctx.Done(): cancel() 退出该goroutine


// 读取数据
n, err := c.conn.Read(buffer)


note:
    c.conn.Read 如果没有数据 则会阻塞 如果c.ctx.Done() 被关闭也不能立即返回
    所以我们还是要判断 read 返回的err 判断底层的网络错误


if n > 0 && c.hc != nil {
	c.updateActivity()
}
更新心跳活动时间



// 是否启用frameDecoder server.IDecoder 设置消息格式
if c.frameDecoder != nil {

}else{

}



defer func() {
		c.Stop() //c.cancel()
		c.doClose()
	}()

```

## StartWriter
```golang
// StartWriter is the goroutine that writes messages to the client
// (写消息Goroutine， 用户将数据发送给客户端)
func (c *Connection) StartWriter() {
	zlog.Ins().InfoF("Writer Goroutine is running")
	defer func() {
		zlog.Ins().InfoF("%s [conn Writer exit!]", c.RemoteAddr().String())
		c.Flush()
	}()
	for {
		select {
		case data, ok := <-c.msgBuffChan:
			if !ok {
				zlog.Ins().ErrorF("msgBuffChan is Closed")
				return
			}

			if err := c.SendBuf(data); err != nil {
				zlog.Ins().ErrorF("Send Buff Data error:, %s Conn Writer exit", err)
				return
			}
			// 一次性循环读出 msgBuffChan 中所有剩余数据
		drainLoop:
			for {
				select {
				case extra, ok2 := <-c.msgBuffChan:
					if !ok2 {
						zlog.Ins().ErrorF("msgBuffChan is Closed")
						return
					}
					if err := c.SendBuf(extra); err != nil {
						zlog.Ins().ErrorF("Send Buff Data error:, %s Conn Writer exit", err)
						return
					}
				default:
					break drainLoop
				}
			}
			// 批量写入完成后一次性 flush
			if err := c.Flush(); err != nil {
				zlog.Ins().ErrorF("Flush Buff Data error: %v Conn Writer exit", err)
				return
			}
		case <-c.ctx.Done():
			return
		}
	}
}


for{
 select{
    case extra, ok2 := <-c.msgBuffChan:
    // 处理待发送消息
   case <-c.ctx.Done():// 上下文被取消
    return
 }


}



c.Send
c.Flush
先把数据写入bufio.Writer 在Flush刷入到底层连接




```

## doClose
```golang
func (s *Connection) doClose() {
	s.finalizer()
	// 归还workerid
	freeWorker(s)
}


```

## finalizer
```golang
func (c *Connection) finalizer() {
	// Call the callback function registered by the user when closing the connection if it exists
	// (如果用户注册了该连接的	关闭回调业务，那么在此刻应该显示调用)
	c.callOnConnStop()

	// Stop the heartbeat detector associated with the connection
	if c.hc != nil {
		c.hc.Stop()
	}

	// Close the socket connection
	_ = c.conn.Close()

	// Remove the connection from the connection manager
	if c.connManager != nil {
		c.connManager.Remove(c)
	}

	go func() {
		defer func() {
			if err := recover(); err != nil {
				zlog.Ins().ErrorF("Conn finalizer panic: %v", err)
			}
		}()

		c.InvokeCloseCallbacks()
	}()

	zlog.Ins().InfoF("Conn Stop()...ConnID = %d", c.connID)
}

// 调用hookonConnStop
// 关闭心跳检测
//关闭底层网络连接
//从连接管理器中删除当前连接
// go routine 执行注册 InvokeCloseCallbacks 回掉函数
hook or  InvokeCloseCallbacks 请求会话 等操作


```