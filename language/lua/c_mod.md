# c_mod

## makefile
```

CC=gcc
INCLUDE=../lua-5.5.1/src
all:custom.so


custom.so:custom.c
	$(CC)  $^ -o $@ -I$(INCLUDE) -fPIC -shared






```

## cutsom.c
```c

// 引入可用的lua头文件
#include "lualib.h"
#include "lauxlib.h"



// 模块函数列表
static const luaL_Reg customlib[] = {
    {"print_hello", print_hello},
    {"max", max},
    {NULL, NULL}};

//模块注册
// c 模块
LUAMOD_API int luaopen_custom(lua_State *L)
{
    // 将表中的函数注册进去
    luaL_newlib(L, customlib);

    return 1;
}


```

## test.lua
```lua
// c 动态库 路径搜索路径设置
package.cpath="./?.so"



// 引入模块调用对应的模块
local mod=require("custom");

mod.print_hello()
print(mod.max(100,200))

```