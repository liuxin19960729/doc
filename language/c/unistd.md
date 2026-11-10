# unistd

## exec
```c
原理:
    加载执行程序替换当前进程的程序映像


pid 保持不变
代码端 数据端 对 栈等用户空间内存数据被替换


Linux/POSIX 提供了一组 exec 函数

区别:
    l:参数通过可变参数列表传递
    v:参数通过数组传递
    p：通过 PATH 环境变量搜索可执行文件（Path）
    e：允许显式指定新程序的环境变量（Environment）


int execl(const char *path,const char *arg,... /* (char *) NULL */);
   
   path:程序路径
   arg[0]:程序名 必须填


int execv(const char *path, char *const argv[]);

    path:程序路径


int execle(const char *path,const char *arg,
           ... /* (char *) NULL, char *const envp[] */);

和 execl() 类似，但可以指定新程序的环境变量。



int execve(const char *pathname,
           char *const argv[],
           char *const envp[]);



envp:

char *envp[] = {
    "APP_ENV=production",
    "SERVER_PORT=8080",
    "PATH=/usr/bin:/bin",
    NULL
};


int execlp(const char *file,
           const char *arg,
           ... /* (char *) NULL */);

例如 
  execlp("ls", "ls", "-l", (char *)NULL);
  会自动到 PATH 环境变量中查找 ls 程序



int execvp(const char *file, char *const argv[]);


例如
char *argv[] = {
    "ls",
    "-l",
    NULL
};

execvp("ls", argv);

  会自动到 PATH 环境变量中查找 ls 程序



note:
   1.这些函数只有出错才会返回
    2.环境变量和 参数 使用NULL 表示结束
execve() 系统调用 其他5个函数都是调用该函数

int execve(const char *pathname,
           char *const argv[],
           char *const envp[]);


```