# stdio
## sprintf
```c
#include <stdio.h>

int sprintf(char *str, const char *format, ...);


str:接收格式化字符串的目标缓冲区
format:格式化字符串
...:可变参数列表
```

## snprintf
```c
int snprintf(
    char *str,
    size_t size,
    const char *format,
    ...
);


str:缓冲区
szie:缓冲区容量包含\0
format:格式化字符串
... 可变参数列表

return 
    < 0 发生错误
    >=0 返回字符串长度(不包含\0)



```
## fgets
```c
#include <stdio.h>

char *fgets(char *str, int n, FILE *stream);

str 缓冲区
n 最多读取n-1个字符(最有一个字符特意\0)
stream 文件流


return 
   str 成功
   NULL 失败
   EOF 没有读取到任何字符返回NULL


```

## fputs
```c
int fputs(const char *str, FILE *stream);

str 要写入的字符串


return 
    成功 > 0
    失败 返回EOF


note:
  fputs 不会在字符串末尾添加\0 或\n

```