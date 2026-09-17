# juc
## Future
```java
java 5 新加的接口

interface Runnable
interface Future<V>

interface RunnableFuture<V>  继承了上面两个接口

RunnableFuture<V> 实现类
Class FutureTask<V>


FutureTask(Runnable runnable, V result)

Creates a FutureTask that will, upon running, execute the given Runnable, and arrange that get will return the given result on successful completion.



get()
get(long timeout, TimeUnit unit)
   TimeoutException 任务超时
isDone() 是否完成
优点:
    配合线程池显著提高程序效率
缺点:
    1.get() 非常容易导致阻塞
    2.轮询容器导致CPU空转浪费性能

```

## CompletableFuture
```java
jdk8

CompletionStage<T>, Future<T>

class CompletableFuture<T>

interface CompletionStage<T>
代表一步计算过程中的某一个阶段,一个阶段完成会触发另一个阶段


class CompletableFuture<T> 对象创建 
1.直接使用构造函数创建 (下面是官方的解释直接创建的对象是不完整的)
    Creates a new incomplete CompletableFuture.

2.使用4个静态方法创建
static CompletableFuture<Void> runAsync(Runnable runnable)
static CompletableFuture<Void> runAsync(Runnable runnable, Executor executor)
static <U> CompletableFuture<U> supplyAsync(Supplier<U> supplier)
static <U> CompletableFuture<U> supplyAsync(Supplier<U> supplier, Executor executor)

未指定线程池 使用ForkJoinPool.commonPool() 线程池


```