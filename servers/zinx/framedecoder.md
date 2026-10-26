# framedecoder
```
FrameDecoder：负责从连续的字节流中切出完整的数据帧。

TLVDecoder：负责解析数据帧中的 Tag、Length 和 Value。
```

## type FrameDecoder struc
```golang
type FrameDecoder struct {
	ziface.LengthField // Basic properties inherited from ILengthField

	LengthFieldEndOffset   int   // Offset of the end position of the length field (LengthFieldOffset+LengthFieldLength) (长度字段结束位置的偏移量)
	failFast               bool  // Fast failure (快速失败)
	discardingTooLongFrame bool  // true indicates discard mode is enabled, false indicates normal working mode (true 表示开启丢弃模式，false 正常工作模式)
	tooLongFrameLength     int64 // When the length of a packet exceeds maxLength, discard mode is enabled, and this field records the length of the data to be discarded (当某个数据包的长度超过maxLength，则开启丢弃模式，此字段记录需要丢弃的数据长度)
	bytesToDiscard         int64 // Records how many bytes still need to be discarded (记录还剩余多少字节需要丢弃)
	in                     []byte
}



LengthFieldEndOffset//长度字段结束位置的偏移量
Offset+Lenght

failFast 是否发现超长frame 失败
discardingTooLongFrame 是否开启丢弃模式
 tooLongFrameLength  记录超长帧的总长度        
bytesToDiscard  还需要丢弃多少字节


in 非常重要 用于 积累数据 用于拼接成完整的数据fram


```

## NewFrameDecoder
```golang
func NewFrameDecoder(lf ziface.LengthField) ziface.IFrameDecoder {

	frameDecoder := new(FrameDecoder)

	if lf.Order == nil {
		frameDecoder.Order = binary.BigEndian
	} else {
		frameDecoder.Order = lf.Order
	}
	frameDecoder.MaxFrameLength = lf.MaxFrameLength
	frameDecoder.LengthFieldOffset = lf.LengthFieldOffset
	frameDecoder.LengthFieldLength = lf.LengthFieldLength
	frameDecoder.LengthAdjustment = lf.LengthAdjustment
	frameDecoder.InitialBytesToStrip = lf.InitialBytesToStrip

	//self
	frameDecoder.LengthFieldEndOffset = lf.LengthFieldOffset + lf.LengthFieldLength
	frameDecoder.in = make([]byte, 0)

	return frameDecoder
}


```

## getUnadjustedFrameLength
```golang
// 获取消息的长度

func (d *FrameDecoder) getUnadjustedFrameLength(buf *bytes.Buffer, offset int, length int, order binary.ByteOrder) int64 {
	// Value of the length field (长度字段的值)
	var frameLength int64

	arr := buf.Bytes()
	arr = arr[offset : offset+length]

	buffer := bytes.NewBuffer(arr)

	switch length {
	case 1:
		//byte
		var value uint8
		binary.Read(buffer, order, &value)
		frameLength = int64(value)
	case 2:
		//short
		var value uint16
		binary.Read(buffer, order, &value)
		frameLength = int64(value)
	case 3:
		// int occupies 32 bits, here take out the last 24 bits and return as int type
		// (int占32位，这里取出后24位，返回int类型)
		if order == binary.LittleEndian {
			n := uint(arr[0]) | uint(arr[1])<<8 | uint(arr[2])<<16
			frameLength = int64(n)
		} else {
			n := uint(arr[2]) | uint(arr[1])<<8 | uint(arr[0])<<16
			frameLength = int64(n)
		}
	case 4:
		//int
		var value uint32
		binary.Read(buffer, order, &value)
		frameLength = int64(value)
	case 8:
		//long
		binary.Read(buffer, order, &frameLength)
	default:
		panic(fmt.Sprintf("unsupported LengthFieldLength: %d (expected: 1, 2, 3, 4, or 8)", d.LengthFieldLength))
	}
	return frameLength
}

```

## decode
```golang
func (d *FrameDecoder) decode(buf []byte) []byte {
	in := bytes.NewBuffer(buf)

	// Determine if it is in discard mode (判断是否为丢弃模式)
	if d.discardingTooLongFrame {
		d.discardingTooLongFrameFunc(in)
	}

	// Determine if the number of readable bytes in the buffer is less than the offset of the length field
	// (判断缓冲区中可读的字节数是否小于长度字段的偏移量)
	if in.Len() < d.LengthFieldEndOffset {
		// Indicates that the length field packets are incomplete, half package
		// (说明长度字段的包都还不完整，半包)
		return nil
	}

	// --> If execution reaches here, it means that the value of the length field can be parsed <--
	// (执行到这，说明可以解析出长度字段的值了)

	// Calculate the offset of the length field
	// (计算出长度字段的开始偏移量)
	actualLengthFieldOffset := d.LengthFieldOffset

	// Get the value of the length field, excluding the adjustment value of lengthAdjustment
	// (获取长度字段的值，不包括lengthAdjustment的调整值)
	frameLength := d.getUnadjustedFrameLength(in, actualLengthFieldOffset, d.LengthFieldLength, d.Order)

	// If the data frame length is less than 0, it means it is an error data packet
	// (如果数据帧长度小于0，说明是个错误的数据包)
	if frameLength < 0 {
		// It will skip the number of bytes of this data packet and throw an exception
		// (内部会跳过这个数据包的字节数，并抛异常)
		d.failOnNegativeLengthField(in, frameLength, d.LengthFieldEndOffset)
	}

	// Apply the formula: Number of bytes after the length field = value of the length field + lengthAdjustment (应用公式:长度字段后的字节数=长度字段的值+长度调整值)
	// frameLength is the value of the length field, plus lengthAdjustment equals the number of bytes after the length field (lengthFieldEndOffset is lengthFieldOffset+lengthFieldLength)
	// (frameLength 是长度字段的值,加上长度调整值等于长度字段后的字节数,lengthFieldEndOffset 是长度字段的偏移量加上长度字段本身)
	// So the frameLength calculated in the end is the length of the entire data packet (那说明最后计算出的frameLength就是整个数据包的长度)
	frameLength += int64(d.LengthAdjustment) + int64(d.LengthFieldEndOffset)

	// Discard mode is turned on here (丢弃模式就是在这开启的)
	// If the data packet length is greater than the maximum length (如果数据包长度大于最大长度)
	if uint64(frameLength) > d.MaxFrameLength {
		// It has exceeded the maximum length of a single data frame, and the exceeded part is processed
		// (已经超过单次数据帧最大长度，对超过的部分进行处理)
		d.exceededFrameLength(in, frameLength)
		return nil
	}

	// --> If execution reaches here, it means normal mode <--
	// (执行到这, 说明是正常模式)

	// Size of the data packet (数据包的大小)
	frameLengthInt := int(frameLength)
	// Determine if the number of readable bytes in the buffer is less than the size of the data packet (判断缓冲区可读字节数是否小于数据包的字节数)
	if in.Len() < frameLengthInt {
		// Half package, will parse again later (半包，等会再来解析)
		return nil
	}

	// --> If execution reaches here, it means that the buffer already contains the entire data packet <--
	// (执行到这, 说明缓冲区的数据已经包含了数据包)

	// Whether the number of bytes to be skipped is greater than the length of the data packet (跳过的字节数是否大于数据包长度)
	if d.InitialBytesToStrip > frameLengthInt {
		// Will throw an exception if the length of the data packet is less than the number of bytes to be skipped (如果数据包长度小于跳过的字节数，将抛出异常)
		d.failOnFrameLengthLessThanInitialBytesToStrip(in, frameLength, d.InitialBytesToStrip)
	}

	// Skip the initialBytesToStrip bytes (跳过initialBytesToStrip个字节)
	in.Next(d.InitialBytesToStrip)

	// Decode (解码)
	// Get the real data length after skipping (获取跳过后的真实数据长度)
	actualFrameLength := frameLengthInt - d.InitialBytesToStrip

	// Extract the real data (提取真实的数据)
	buff := make([]byte, actualFrameLength)
	_, _ = in.Read(buff)

	return buff
}

```