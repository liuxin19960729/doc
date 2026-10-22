# reflect
```
reflect 包
Type 描述类型 Value 描述值
```
## reflect.ValueOf
```

v.Type() 获取类型
v.Kind() 获取底层类型
v.Elem()
    用于指针


v.Len()
  返回数据v的长度
  v 的类型必须还是 Array Map Slid3 String or 指向数组的指针

  否则会引发panic




v.IsNil()
  指针、接口、Map、切片、通道和函数等支持 nil 的类别，

v.IsVaild() 是否有效
```