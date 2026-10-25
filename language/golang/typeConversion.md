# type conversion
## 基本类型转换
```golang

var a int =10

var b float64 = float64(a)

var c int64 = int64(b)

var d uint = uint(c)


note:浮点数转整数直接截断小数部分


```
## 字符串和数字转换
### 字符串转整数
```golang
//str->int
strconv.Atoi
// str->int 64
strconv.ParseInt
```
### 整数转字符串
```golang

strconv.
strconv.FormatInt
```

### 浮点数与字符串互转
```
strconv.ParseFloat
strconv.FormatFloat
```
### 类型断言：interface 转具体类型
```golang
n, ok := v.(int) // ok 表示是否转换成功
```