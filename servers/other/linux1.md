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
      p xx = x 也可以设置变量的值
set  var   设置变量的值


tui disable 关闭 tui 
info b 查看所有断点
delete 删除所有断点
delete n 删除编号为n的断点
bt 查看函数调用栈



```
## gdb 调试core 文件
```
ulimit -a 查看限制内容

ulimite -c unlimited   // 打开core文件限制

修改 ulimit 
    ulimit 选项  选项值


gdb 程序文件名 core文件名

```
## gdb调试正在运行的程序
```
ps -ef | grep 程序名

获取程序的进程编号

gdb 程序名 -p 进程编号

```
## linux 的时间操作
```c
c++11 提供类操作时间的chrono库

time.h 
    定义 time_t ,long 类型的别名 

// 当前时间获取(1970-1-1到现在的秒数)
time_t now1=time(0);
time_t now2;
time(&now2);


// 时间处理
    // 系统当前时间
    time_t now1 = time(0);

    tm tmnow;
    // localtime 非线程安全 localtime_r 线程安全函数
    localtime_r(&now1, &tmnow);

    std::string year = std::to_string(tmnow.tm_year + 1900);
    std::string month = std::to_string(tmnow.tm_mon + 1);
    std::string day = std::to_string(tmnow.tm_mday);

    std::string h = std::to_string(tmnow.tm_hour);
    std::string m = std::to_string(tmnow.tm_min);
    std::string s = std::to_string(tmnow.tm_sec);

    std::cout << year << "-" << month << "-" << day
              << " " << h << "::" << m << "::" << s << std::endl;



// 将 tm 转换为 time_t
 time_t mktime(struct tm *tm);





#include<unistd.h>
// 将程序挂起一段时间
sleep(s); // 单位秒
usleep(um);//单位微秒 1s=1000ms=1000000us


```

## 文件操作函数
### 目录操作函数
```c

获取当前的工作目录
#include <unistd.h>

char * getcwd(char *buf, size_t size)
char * get_current_dir_name();
    该内存需要调用者释放

note:
    linux 中目录的最大长度为255


切换工作目录
#include <unistd.h>
int chdir(const char *path)
return 
     0 成功
     其他 失败(目录不存在或权限不足)



// 创建目录
#include<sys/stat.h>
mkdir(path,mode) 
    note:
           1. mode 0755 不要省略前面的0
           2. 上级目录不存在不能创建

//删除目录
#include <unistd.h>

int rmdir(const char *path)

return
    0 成功
    其他 失败(目录不存在或权限不足)



```
### 获取文件中目录的列表
```c

#include <dirent.h>

DIR * opendir(const char *path);
    return 
        NULL 失败 errno 错误原因

reddir(Dir *dir)

return  
    ! NULL 成功
    NULL 失败 errno 错误原因


// 关闭目录指针
closedir(Dir *dir)


struct dirent
{
    
    unsigned char d_type;// 8 常规文件  4 目录
    char d_name[];  // 文件名 or 目录名
}

```

## Linux 的系统错误
```cpp

#include<string>
strerror() 和 perror() 可以查看出错的详细信息

gcc 8.3.1有 133个错误代码


errno 只有在发生错误的才会被设置,不会被清除,不能根据errno 来判断是否有错误


```

## 目录和文档的更多操作
```cpp

access()
   判断目录和文件的存储权限
   
   // 判断 目录 获取文件是否存在 
   access(path,F_OK)!==0



stat()
    获取文件的状态信息
    实际开发中 关注 st_mode st_size st_mtine(文件最后一次被修改的时间) 等成员



utime()


rename() 从命名文件 或目录


remove() 删除目录或文件


```
## Linux 信号
```cpp
kill 
killall 
   向进程发送信号 

   killall -信号种类 



信号的处理
1.对信号的处理采用系统默认操作,大部分信号默认操作是终止进程
2.设置中断处理函数 接收到信号 由函数处理
    signal(signum,handle)
    ......
3.忽略信号 接收到信号 不做任何处理
    signal(signum,SIG_IGN)



// 注册了信号处理函数
void sig_handle(int signum)
{
    std::cout << "signum:" << signum << std::endl;
}

int main()
{

    signal(1, sig_handle);
    signal(5, sig_handle);

    while (1)
    {
        if (sleep(5) != 0)
        {
            perror("sleep");
            continue;
        }
        std::cout<<"sleep timeout"<<std::endl;
    }
}

重要信号

SIGINT 2  CTRL+C
SIGKILL 9 KILL KILLALL  -9  强制杀死程序
      信号默认终止程序
      信号不能被捕获
      信号不能被忽略

SIGSEGV 11 无效的内存引用
    不能被捕获和忽略


SIGALRM 14  由闹钟alarm() 发出的信号
        alarm(5) 5秒后向自己发送信号
    

SIGTERM 15  由kill 或 killall 发出的信号
     
SIGCHLD 17  子进程退出时发出的信号



信号作用:
    1.程序可以监听信号 来做一些处理 然后退出
    2.向程序发送0信号可以检测程序是否存活
   
```
## 进程终止
```cpp

终止进程
#include <stdlib.h>
exit(0);

_exit()  _Exit()




异常终止三种方式
1.程序调用abort()
2.接收到信号
3.最后一个线程对取消请求做出回应


shell echo $? 查看进程终止的状态



exit 
   析构函数不会调用 只会调用全局的析构函数


exit()
    1.将缓冲区的数据写入磁盘,等做一些善后的工作

_exit _Exit 直接退出不会做任何善后工作
    note: 连全局的析构函数都不会调用




atexit() 注册一个函数 在进程退出时调用
   正常返回 和exit() 才会调用该函数
   note:
    _exit() _Exit() 不会调用
    注册的函数 按照注册的顺序逆序调用

```

## 调用可执行程序
```cpp
#include <stdlib.h>
int system(const char *command);

return 
    !0 执行程序不存在
    0 执行成功 程序返回 0
    ！0 执行成功 程序返回非0

execve 函数族
```

## 进程创建
```
共享文件
1.父进程和子进程共享同一个文件偏移量
```
## 僵尸进程
```cpp
父进程先于子进程退出,子进程会被1号进程托管


将程序运行到后台
1../程序  &
2.fork() 父进程 直接退出



如何避免僵尸进程
1.父进程忽略SIGCHILD 信号
    signal(SIGCHILD,SIG_IGN)
2.wait/waitpid 等待子进程结束
  WIFEXITED(status) 判断是否是正常退出
    WEXITSTATUS(status) 获取退出状态
  
  WTERMSIG(status) 异常退出获取异常退出的信号


3.捕获SIGCHILD 并且在捕获函数中调用wait/waitpid



```

## 进程发送信号
```cpp
kill(pid,sig)

1.pid>0 发送进程号为pid的进程
2.pid=0 发送当前进程组的所有进程,note:当前发送进程自己能接收到自己的信号
3.pid=-1 广播给系统所有进程,例如系统关机时会向所有登录窗口广播关机信息

sig：  
    0 不会发送任何信号 但是可以通过 0 来检测某个进程是否在运行


return 
    0 成功
    -1 失败 并设置 errno



多进程信号处理

1.子进程接收到信号自己退出
   signal(SIGTERM,SIG_IGN);
   signal(SIGINT,SIG_IGN);
   // 释放资源
    exit()
2.父进程收到信号 给子所有子进程发送退出信号 ,自己在退出
    signal(SIGTERM,SIG_IGN);
    signal(SIGINT,SIG_IGN);
    kill(0,SIGTERM);
    // 释放资源
    exit(0)
```

## 共享内存
```cpp
共享内存没有提供锁机制,如果要对共享内存的读写加锁可以使用信号量

#include <sys/shm.h>
int shmget(key_t key, size_t size, int shmflg);
    用于创建/获取共享内存



key:
    用16进制增加刻度性


size:byte 为单位

shmflg:
    0666 和文件权限一样 rw权限
    IPC_CREAT 没有就创建


return
    >0 成功 返回共享内存id
    -1 失败 并设置 errno
       系统内存不足或没有权限



ipcs -m 可以查看系统的共享内存



       #include <sys/shm.h>



void *shmat(int shmid, const void *_Nullable shmaddr, int shmflg);
共享内存连接到当前进程的地址空间


int shmdt(const void *shmaddr);
  把共享内存从当前进程中分离


```