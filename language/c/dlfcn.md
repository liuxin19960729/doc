# dlfcn
## #include <dlfcn.h>
```c

允许在程序运行过程中加载.so动态库,而不是在编译或连接阶段就确定依赖关系

void *dlopen(const char *filename, int flags);


filename:动态库路径
   例如:./libtest.so

flags:加载方式

return
    NULL 失败




flags:


RTLD_LAZY	延迟解析函数符号，使用时再解析
RTLD_NOW	立即解析所有未定义符号
RTLD_GLOBAL	将库中的符号暴露给后续加载的动态库
RTLD_LOCAL	符号只在当前加载对象的局部作用域可见




dlclose(dlopen返回值) 卸载 


```

## dlsym
```cpp

更具动态库的句柄和名查找全局变量 并且返回他们的地址


void *dlsym(void *handle, const char *symbol);


```

## dlerror()
```c
最近一次加载的错误信息


```

## note
```
cpp 里面使用

#include <iostream>

// 按照c语言的方式s
extern "C" void xxxx()
{
   
}



这样编译的时候才不会按照cpp编译规则从新修改名称导致找不到



```