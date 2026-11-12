# mmap
```c
文件,设备或则匿名内存映射到进程虚拟地址空间

映射之后返回虚拟内存地址,可以像直接操作内存一样操作这写映射的东西



note:mmap 并不会把整个文件都加载到物理内存中,通常读取到缺页异常操作系统才会加载对应内存页



#include <sys/mman.h>

void *mmap(
    void *addr,
    size_t C,
    int prot,
    int flags,
    int fd,
    off_t offset
);


return
    成功:返回映射区域的起始虚拟地址

    失败:MAP_FAILED(-1)



addr:
    1.指定映射到那个虚拟地址,一般传NULL

    NULL:让Linux 内核自行选择合适的虚拟地址
    非NULL:
         1.普通情况,它只是地址一个提示
         2.MAP_FIXED:强制映射到指定地址



C:申请映射字节数
  note: 实际映射区域会按页大小进行对齐和处理。
  

  sysconf(_SC_PAGESIZE) // 查看系统页大小



prot:
     内存访问权限

PROT_READ	允许读取
PROT_WRITE	允许写入
PROT_EXEC	允许执行
PROT_NONE	不允许访问

note:权限还会收到文件打开模式和底层对象系统安全策略等因素限制


flags:
    它决定映射区域的行为，尤其是修改数据后是否会影响其他映射或文件。



note:修改文件内存内容会立即写回到内核页缓存但是不能立即持久化保持到磁盘

MAP_SHARED	共享映射
  多个进程映射同一文件可以共享修改结果
   

MAP_PRIVATE	私有映射，写时复制
  私有映射，采用 Copy-On-Write（写时复制）机制


MAP_ANONYMOUS	匿名映射
MAP_FIXED	指定固定映射地址
MAP_POPULATE	尝试预先建立页面映射


offset:
    offset 必须是系统页大小的整数倍

```

## mmap 的底层工作原理
```
内核主要做的工作
1.在当前进程建立虚拟映射地址空间(VMA)
2.记录映射区域的权限,文件对象,偏移等信息


note:mmap() 返回并灭有加载文件

只有当读或修改文件内容触发 Page Fault 缺页异常进入内核缺页处理流程,内核找缓冲或到存储设备读取 建立页表映射 返回用户台从新执行访问指令



```
## munmap
```c
#include <sys/mman.h>

int munmap(void *addr, size_t length);


解除映射




```

## msync
```c
#include <sys/mman.h>

int msync(void *addr, size_t length, int flags);

同步映射数据


flags
参数	含义
MS_SYNC	同步等待写回完成
MS_ASYNC	请求异步写回
MS_INVALIDATE	请求使其他映射副本失效




```