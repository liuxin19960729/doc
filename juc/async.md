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
### CompletableFuture 方法说明
```java
1.获得结果并且触发计算

public T get() throws InterruptedException,ExecutionException
public T get(long timeout, TimeUnit unit) throws InterruptedException,ExecutionException,TimeoutException
public T join() Throws:
CancellationException - if the computation was cancelled
CompletionException - if this future completed exceptionally or a completion computation threw an exception
public T getNow(T valueIfAbsent)
    该函数不会阻塞 调用该函数 如果计算完成 返回 计算完成值 or 抛出异常 否则 返回valueIfAbsent


public boolean complete(T value) 
 是否打断get join 方法 返回指定的值


2.对计算结果进行处理
public <U> CompletableFuture<U> thenApply(Function<? super T, ? extends U> fn)
note:如果存在多个步骤 ,在中间某一个步骤出现异常就直接抛出异常 不进行下一步骤
public <U> CompletableFuture<U> handle(BiFunction<? super T, Throwable, ? extends U> fn)
note: 如果存在多个步骤 中间步骤 出现异常 会继续执行下一步骤 会将上一个出现的异常传给下一步
对计算结果存在依赖,两个线程串行化


3.对计算结果进行消费
public CompletableFuture<Void> thenAccept(Consumer<? super T> action) 
  对上一步计算的结构进行消费无返回结果


4.对计算速度的选用
public <U> CompletableFuture<U> applyToEither(CompletionStage<? extends T> other,Function<? super T, U> fn)

5.计算结果合并
public <U,V> CompletableFuture<V> thenCombine(CompletionStage<? extends U> other,BiFunction<? super T, ? super U, ? extends V> fn)

两个任务执行完返回一个新的 CompletableFuture

note:
   xxxAsync 方法 可以指定线程池执行该任务

   1.不传入线程池使用ForkJoinPool
   2.如果第一个任务传入一个自定义线程池
     xxx 执行第二个任务和第一个任务共用一个线程池
     xxxAsync   第一个使用的时自定义线程池 第二使用 ForkJoinPool or 显示传入的线程池

  3.如果任务处理的太快,系统优化切换原则,可能直接使用main 线程池处理

```