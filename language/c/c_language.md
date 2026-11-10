# c language

## 可变参数
```c
#include <stdarg.h>
va_list 可变参数列表

void va_start(va_list ap, last);
 参数初始化读取
type va_arg(va_list ap, type);
   获取下一个参数
void va_end(va_list ap);
    结束参数读取
void va_copy(va_list dest, va_list src);
    复制参数读取状态




```