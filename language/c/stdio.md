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