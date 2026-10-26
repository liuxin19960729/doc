## server
```golang

type Server struct{


}


packet ziface.IDataPack // 数据保温封包方式
decoder ziface.IDecoder //粘包解码器



```

## newServerWithConfig
```golang
服务器配置初始化

packet: zpack.Factory().NewPack(ziface.ZinxDataPack),
decoder:zdecoder.NewTLVDecoder()


// 可以将packet 设置成自定义的
func (s *Server) SetPacket(packet ziface.IDataPack)

// 可以将decoder 设置成自定义的
func (s *Server) SetDecoder(decoder ziface.IDecoder)

```

## StartConn
```golang
func (s *Server) StartConn(conn ziface.IConnection) {
	// HeartBeat check
	if s.hc != nil {
		// Clone a heart-beat checker from the server side
		heartBeatChecker := s.hc.Clone()// 心跳检查器

		// Bind current connection
		heartBeatChecker.BindConn(conn) 
	}

	// Start processing business for the current connection
	conn.Start()
}

```