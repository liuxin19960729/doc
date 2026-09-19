# volatile
```
1.可见性
    写一个volatile 会将线程的工作内存数据立刻刷新会主内存
    读一个volatile 会将线程的工作内存该数据设置为无效 直接从主内存读取数据
2.有序性
   该变量不允许指令重排
   通过内存屏障技术禁止指令重排序

```
## volatile 可见性保证
```
1.非volatile 修改 值何时刷入到主线程无法预测
2.如果该值已经被刷新到主线程,但是还是一直在使用工作内存的值

volatile 每次修改 都会刷新到主内存,每次读取volatile 变量都会死从主内存中读取最新值


wire 
lock 主内存
写入数据到主内存
清空其他线程里面使用到该volatile变量的值
unlock 主内存



note: volatile 只能保证可见性和有序性 不能保证原子性

```
## 指令禁止重排
```

storestore
valatile 写  -- 保证 valatile 写之前的所有普通写操作都刷新到内存
storeload  --

valatile 写 是第二个操作 普通读写 和valatile 读 都不允许重排序
valatile 写 后面的volatile 读写都不允许从排序



volatile 读
loadload 
loadstore 
    --禁止下面的普通读写和volatile 读重排序


volatile 写之前的任何操作不允许重排序到后面
volatile 读之后的操作不允许从排到前面
```
## volatile 适用场景
```
1.状态标记 判断业务是否结束
2.单例模式 
```