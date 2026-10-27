# fs
```
go 文件读取主要围绕 os,io,bufio,io/fs 这几个包展开


体系:
os
    文件打开 创建 删除 *os.File

io 
    Reader Writer 同样接口

bufio
    缓冲读取,按行 字节等读取

io/fs
    统一描述文件系统和文件读取能力
```
## 读取文件常用API
```golang


os.ReadFile 一次性读取整个文件  适合小型配置文件，JSON
os.Open     打开文件返回 *os.File  分块读取大文件
os.OpenFile 执行打开标志和权限    读写和创建

File.Read  读取一段字节  二进制文件 流式处理

bufio.NewReader 自带缓冲去读取器  按行 按字节读取
bufio.Scanner 按行自定义扫描   日志 文本处理
io.ReadAll 读取整个io.Reader  http响应体 内存流
io.Copy 将Reader赋值到Writer 文件赋值和流转发


```

### os.ReadFile
```golang

// 适合小文件一次读取完

bytes, err := os.ReadFile("fs/config")
if err != nil {
	log.Fatal(err)
}
fmt.Printf("原始数据: %v\n", bytes)
fmt.Printf("文本内容:\n%s", bytes)
```


### os.Open + File.Read 分块读
```golang
	file, err := os.Open("fs/config")

	if err != nil {
		log.Fatal(err)
	}

	buf := make([]byte, 4)
	content := make([]byte, 0)
	for {
		n, err := file.Read(buf)

		if n > 0 {
			content = append(content, buf[:n]...)
		}

		if err == io.EOF {
			break
		}

		if err != nil {
			log.Fatal(err)
		}
	}

	fmt.Printf("文本内容:\n%s", content)


note:
    先出n在处理err,因为读取可能返回有效数据和err为EOF

```
### io.Reader：Go 读取体系的核心接口
```golang
type Reader interface {
    Read(p []byte) (n int, err error)
}


只要一个对象实现Read犯法就可以作为数据源交给其他读取函数使用


*os.File
bytes.Buffer
string.Reader
Http 响应体
netConn 
..... 这些都实现了Read 函数


// 如果实现这个函数 编写一个函数 将io.Reader 作为类型那么实现
// 这个函数的兑现都能传入该函数


io.ReadAll(io.Reader) 就是实现Read函数都能丢入里面 获取全部的内容



// 将 io.Reader 实现的对象 放入NewScanner 一行一行的读取
scanner := bufio.NewScanner(file)
for scanner.Scan() {
	fmt.Println(scanner.Text())
}
if err := scanner.Err(); err != nil {
		log.Fatal(err)
}

note:
    Scanner 默认单个token 最长64kb
    
    // 自定义设置 token 长度 和buf
    scanner.Buffer(buf,maxToeknSize)




// 将 io.Reader 实现的对象 放入NewReader 按行读取
	reader := bufio.NewReader(file)

	for {
		str, err := reader.ReadString('\n')

		if len(str) > 0 {
			fmt.Print(str)
		}

		if err == io.EOF {
			break
		}
	}


note: Scanner 和 Reader 怎么选
    超长行 Reader
    普通文本 Scanner 和Rader 
    按指定分隔符读取 Reader.ReadString
    读取一个字节	Reader.ReadByte
    读取一个 UTF-8 字符	Reader.ReadRune
```

## os.OpenFile
```golang
	O_RDONLY int = syscall.O_RDONLY // open the file read-only.
	O_WRONLY int = syscall.O_WRONLY // open the file write-only.
	O_RDWR   int = syscall.O_RDWR   // open the file read-write.
	// The remaining values may be or'ed in to control behavior.
	O_APPEND int = syscall.O_APPEND // append data to the file when writing.
	O_CREATE int = syscall.O_CREAT  // create a new file if none exists.
	O_EXCL   int = syscall.O_EXCL   // used with O_CREATE, file must not exist.
	O_SYNC   int = syscall.O_SYNC   // open for synchronous I/O.
	O_TRUNC  int = syscall.O_TRUNC  // truncate regular writable file when opened.




func OpenFile(name string, flag int, perm FileMode) (*File, error) 



os.O_TRUNC	打开时清空已有文件内容

note:
   不要把O_CRUNC 假如到保留原始内容的文件打开参数里


```