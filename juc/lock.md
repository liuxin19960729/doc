# lock
### 悲观锁和乐观锁
```
1.悲观锁
lock synchronized
写操作多的场景


2.乐观锁(无锁机制)(认为自己在使用数据式不会有别的线程来修改数据和或资源)
 1.version 版本号机制
 2.cas 算法
 读操作多的场景

```

## synchronized
```java
同步代码块
monitorenter
monitorexit

普通同步方法 ACC_SYNCHRONIZED 标志被设置

静态同步方法 ACC_STATIC, ACC_SYNCHRONIZED 标志设置


实现
ObjectMoniter.java ObjectMoniter.hpp ObjectMoniter.cpp

ObjectMonitor::ObjectMonitor(oop object) :
  _metadata(0),
  _object(_oop_storage, object),
  _owner(NO_OWNER),
  _previous_owner_tid(0),
  _next_om(nullptr),
  _recursions(0),
  _entry_list(nullptr),
  _entry_list_tail(nullptr),
  _succ(NO_OWNER),
  _SpinDuration(ObjectMonitor::Knob_SpinLimit),
  _contentions(0),
  _unmounted_vthreads(0),
  _wait_set(nullptr),
  _waiters(0),
  _wait_set_lock(0)
{ }

_owner 持有锁的线程指针
_wait_set 状态的线程队列
_recursions 锁的重入次数
_entry_list 等待锁的block线程队列
.....
```
## 公平锁和非公平锁
```java
非公平锁 吞吐量更加高,节省线程切换时间

```
## 可重入锁
```java
synchronized
ReentrantLock
```
## 死锁和死锁的排查
```
jps -l
jstack 进程编号


jconsole 检测死锁


```