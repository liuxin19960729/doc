# LockSupport 和 线程中断
```java



class Thread

public void interrupt()
    java 通过interrupt() 提供了一种协商机制

  此时线程调用 Object wait  Thread join sleep 方法被阻塞的时候调用 thread.interrupt() 被阻塞的线程立即返回并且抛出一个InterruptException 并且清除中断状态

  如果此时线程在InterruptibleChannel 执行IO被阻塞 则执行thread.interrupt() 该通道将被关闭并且接收到 ClosedByInterruptException

  如果 在 Selector 中使用被阻塞调用该方法 则会立即返回 可能返回一个非零值 就像调用选择器的唤醒方法一样

public boolean isInterrupted()
   是否被标记为中断标记
   中断不活动的线程不会被产生任何影响
public static boolean interrupted() 
返回是否被被标记为中断状态并且清除中断标记
```
## LockSupport
```java
static void park()  阻塞线程
static void unpark() 解除被阻塞线程

java 中存在三种等待唤醒方法
1.Object wait notify
   synchronized
   wait 等待 并且释放锁
   wait 和 notify 必须要持有锁才能使用
   当有两个线程 如果 notify 先与wait 执行 则wait 睡眠之后将永远无法醒来
2.JUC 包中的Condition await  signal
    Lock 
3.LockSupport park unpark


```