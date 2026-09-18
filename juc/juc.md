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

// 任务执行完执行该回掉 该回掉会传入两个值 返回值 and 异常信息
public CompletableFuture<T> whenComplete(BiConsumer<? super T, ? super Throwable> action)
// 任务出现异常会调用该回掉函数
public CompletableFuture<T> exceptionally(Function<Throwable, ? extends T> fn)



// 和  get 一样 不用一定补货或则抛出异常 join 返回的异常是运行时的异常
public T join()

note:
Java 异常体系里：

Checked Exception（继承 Exception 但不继承 RuntimeException）：编译器强制要求 try-catch 或 throws，否则编译失败。

Unchecked Exception（继承 RuntimeException）：编译器不检查，写不写 try-catch / throws 都能编译通过。



```
### 函数式接口说明
```java
Runnable void run()
   
// 功能形
Function<T, R>  R get(T t)

// 消费形函数
Consumer<T>  void accept(T t)
    BiConsumer  void accept(T t, U u);

// 供给形
Supplier<T>   T get();
```
