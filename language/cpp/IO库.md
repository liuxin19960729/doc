# IO库
```cpp
C++ 不直接处理输入输出,而是定义在标准库中的类型性来处理IO

类型支持从设备读和写
设备可以是文件和控制窗口等



istream 输入流类型
ostream 输出流类型


cin 输入流对象
cout 输出流对象
cerr ostream 流对象,标准错误输出流

```
## IO 类
```cpp

w 开始支持wchar_t 字符类型

iostream
    istream  wistream
    ostream  wostream
    iostream wiostream


fstream 文件流
    ifstream  wifstream
    ofstream  wofstream
    fstream  wfstream


ssstream 字符串流
    istringstream  wistringstream
    ostringstream  wostringstream
    stringstream  wstringstream

```
### IO 类型间的关系
```cpp
IO 对象流没有复制和拷贝

IO流对象以引用方式传递或返回

IO 流的读写会改变内部的状态 注意传递和发挥不能是 const 引用

```
### 条件状态
```
1.
int ival;

cin>>ival;

输入 字符或结束符 cin 会进入错误状态



while(cin>>ival){//操作成功  返回 true

}

```

#### 查询流的状态
```cpp
iostate:

    badbit 系统级错误 流无法在使用
    failbit  输入失败
        例如 如果期望读取 数字 却读出字符 
        可以修复
    eofbit  文件结束

    goodbit  
        流未发生错误




good()
   true 所有错误位都为0
bad()
fail()
eof()

```

#### 管理条件状态
```
rdstate() 对应流当前状态
setstate()
```