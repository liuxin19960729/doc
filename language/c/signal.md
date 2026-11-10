# signal
## sigaction
```c

注册,修改,查询信号处理行为的系统调用接口

SIGINT (CTRL + C)
SIGTERM 进程终止请求
SIGCHLD 子进程退出
SIGPIPE (向已经关闭的连接写入数据)


int sigaction(
    int signum,
    const struct sigaction *act,
    struct sigaction *oldact
);


signum:要处理信号的编号
act : 新的信号处理配置
oldact : 获取之前处理的配置 可以为NULL



return 
    0 成功
    -1 失败 并设置 errno



struct sigaction {
    void (*sa_handler)(int);

    void (*sa_sigaction)(
        int,
        siginfo_t *,
        void *
    );

    sigset_t sa_mask;

    int sa_flags;

    void (*sa_restorer)(void);
};


sa_handler 普通信号处理函数
sa_sigaction 可以获取更多信号的处理函数
sa_mask：信号处理期间屏蔽那些信号
sa_flags 控制信号的处理行为



// 普通信号的处理函数
void handler(int signum)
{
    printf("收到信号: %d\n", signum);
}



sa_mask：信号屏蔽集合
   正在处理某个信号时,屏蔽那些信号
   note:正在处理的信号默认自动加入集合
     除非在sa_flags 里面设置  SA_NODEFER

int sigemptyset(sigset_t *set);
   // 初始化清空信号集合

int sigfillset(sigset_t *set);
   // 将所有信号加入集合

int sigaddset(sigset_t *set, int signum);
    //添加一个信号
int sigdelset(sigset_t *set, int signum);
    // 删除一个信号

int sigismember(const sigset_t *set, int signum);
    // 判断某个信号是否在集合中



sa_flags:控制信号处理行为


SA_RESTART	尽可能自动重启被信号中断的系统调用
    自动重启系统调用

例如
int n = accept(listenfd, NULL, NULL);
1.accept() 阻塞等待客户端连接
2.进程收到 SIGCHLD
3.执行 SIGCHLD对应的信号处理函数
4.信号处理执行完毕

如果没有设计者 SA_RESTART 
  
  accept() 返回 -1 errno = EINTR



note:
  并不是所有接口都支持自动恢复 要让程序稳定,还是需要判断INTER



SA_SIGINFO	使用三参数信号处理函数
SA_NODEFER	不自动屏蔽当前正在处理的信号
SA_RESETHAND	处理一次信号后恢复默认处理行为
SA_NOCLDSTOP	子进程停止或继续时，不产生对应的 SIGCHLD 通知
SA_NOCLDWAIT	子进程退出时不产生僵尸进程
SA_ONSTACK	使用备用信号栈执行处理函数



```
## SIG_IGN
```c
作用忽略某个信号


信号处理的三种方式
1.SIG_DFL 使用系统默认的方式
2.SIG_IGN 忽略信号
3.自定义函数 自己处理信号



#include <signal.h>
typedef typeof(void (int))  *sighandler_t;
sighandler_t signal(int signum, sighandler_t handler);



// 忽略指定的信号
1.signal(signum,SIG_IGN)
2.
 struct sigaction sig = {0};

    sig.sa_handler = SIG_IGN;

    sigemptyset(&sig.sa_mask);

    sigaction(SIGINT, &sig, NULL);




```