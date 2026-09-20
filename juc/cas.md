# cas
```java
public final boolean compareAndSet(int expectedValue,int newValue)
   返回是否修改成功

   底层使用 cmpxchg 汇编 指令 
   多核的情况下 lock cmpxchg 
   

```
## AtomicReference<V>
```java
final boolean compareAndSet(V expectedValue, V newValue)
```
## CAS 自旋锁
```java
public class SpinLockDemo {
    private final AtomicReference<Thread> atomicReference = new AtomicReference<Thread>(null);


    public void lock() {
        Thread thread = Thread.currentThread();
        while (!atomicReference.compareAndSet(null, thread)) {
        }
        System.out.println(thread.getName() + ":lock");
    }


    public void unlock() {
        Thread thread = Thread.currentThread();
        atomicReference.compareAndSet(thread, null);
        System.out.println(thread.getName() + ":unlock");
    }
}

```
## CAS 的缺点
```java
1.循环时间长开销大
3.ABA 问题
解决ABA问题
class AtomicStampedReference<V>


```