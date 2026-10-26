# bytes
## Buffer 
```
write([]byte)
writeString
writeByte(b byte)
writeRune(r rune) 追加一个 Unicode 码点的 UTF-8 编码




String()
Bytes()

Len()
Cap()


Reset() 重置 buf off lastRead 值重置
Grow() 将空间设置为xxx Cap至少为设置的值


Next(n)
    返回 off,off+n 的数据 
    note: off会被设置为 off=off+n
```