# linux1
## 静态库和动态库
### 静态库的制作和使用
```
静态库制作
g++
-c 只编译
-o 指定文件名
   libxxx.a


静态库的使用
-L  指定静态库的文件目录地址
-l  指定静态库的库名

例如:
g++ app.cpp  -o app -L../public -l pub

```
### 动态库的制作和使用
```
动态库的制作
-fPIC -shared
-fPIC :生成位置无关代码(Position Independent Code)
-shared:生成共享库(Shared Library)
g++ pub.cpp -fPIC -shared -o libpub.so



动态库的使用
g++ app.cpp -o app -L../public -lpub


note:使用的动态库需要设置LD_LIBRARY_PATH 环境变量
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:....

程序在运行的时候载入动态库


动态库升级非常简单 不需要重新编译代码 只需要修改动态库代码重新制作动态库就可以


note:
    动态库和静态库同时存在编译器优先使用动态库
```

## makefile
```

CC=g++

# 指定编译的目标文件
# 多行书写 \ 换行
all: libpub.a	\
		libpub.so

# 编译 libpub.a 依赖  pub.h pub.cpp
#  pub.h pub.cpp 发生变化重新编译 libpub.a
libpub.a: pub.h pub.cpp
	$(CC) -c -o $@  pub.cpp

libpub.so: pub.h pub.cpp
	$(CC)  -fPIC -shared -c -o $@  pub.cpp

clean:
	rm f  libpub.a libpub.so



CC=g++
PUB_DIR=../public/

all:app

# $^ 表示当前所依赖的文件列表
app:app.cpp
	$(CC)  $^ -I$(PUB_DIR) -L$(PUB_DIR) -lpub -o $@

clean:
	rm -f app

run:
	./app




makefile 变量
定义变量
    CC=g++
    PUB_DIR=../public/

使用变量
    $(CC)  $(PUB_DIR)



```

## main 函数参数
```cpp
int main(int argc, char *aragv[], char *envp[])
{
    std::cout << "argc:" << argc << std::endl;

    std::cout << "=========================" << std::endl;
    // argv[0] 程序名
    for (size_t i = 0; i < argc; i++)
    {
        std::cout << "i:" << i << " v:" << aragv[i] << std::endl;
    }

    // std::cout << "=========================" << std::endl;

    // for (size_t i = 0; envp[i] != nullptr; i++)
    // {
    //     std::cout << "i:" << i << " v:" << envp[i] << std::endl;
    // }

    return 0;
}

// 获取和设置环境变量
getenv
setenv
    note:此函数设置的环境变量只对本进程有效


 int setenv(const char *name, const char *value, int overwrite);

overwrite
    0 不覆盖已经存在的环境变量
    非 0 覆盖已经存在的环境变量
```
## gdb
```
调试程序  -g  并且不能使用 -o 的优化选项

gdb 的 常用命令

set args  程序 参数1 参数2 ....

break b 设置断点
run r 开始运行程序
next n 执行当前语句 如果该条语句为函数语句 不会进入到函数中
step s 执行当前语句 如果该条语句为函数语句 会进入到函数中
continue c 继续执行程序 执行到断点处停止
print p 



```