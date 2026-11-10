# wait
## wait
```c
作用:父进程等待子进程结束会后子进程,防止产出僵尸进程


子进程退出不直接消失原因
1.保留子进程一些信息
    PID 退出码 终止原因 资源使用情况.....


2.保留这些信息父进程可以通过wait() 来获得这些信息


#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>


pid_t wait(int *wstatus);

wstatus:用来接收进程退出状态
NULL 不关心子进程退出状态

return
  >0  返回退出进程pid
  -1 失败 并设置 errno


wait()
   1.有子进程 没有任何进程退出 阻塞等待
   2.有子进程退出回收其中一个
   3. 返回 -1  errno == ECHILD 没有可以等待的子进程

```
## waitpid
```c
pid_t waitpid(pid_t pid, int *wstatus, int options);



pid:指定等待pid
  pid=-1 等待任意子进程退出
  pid =0 等待相同进程组的子进程
  pid <-1 等待指定进程组子进程
wstatus:获取子进程退出状态
options:等待选项

0 阻塞等待
WNOHANG 非阻塞等待
WUNTRACED 子进程暂停也返回
WCONTINUED 子进程从暂停状态恢复时也返回

return 
  s>0 返回退出子进程pid
  -1 失败 并设置 errno
  0 使用 WNOHANG，当前没有可报告的状态变化




// 等待指定pid进程
waitpid(pid,&status,0)

// 等待任意子进程
wait(&status) == waitpid(-1,&status,0)


// 等待同一个进程组的子进程
waitpid(0, &status, 0);


// 等待进程组委 ID 1000 的子进程
waitpid(-1000, &status, 0);





```