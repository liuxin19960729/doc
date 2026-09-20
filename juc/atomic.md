# atomic
```
1.基本类型原子类
    AtomicBoolean
    AtomicInteger
    AtomicLong
2.数组类型原子类
    AtomicIntegerArray
    AtomicLongArray
    AtomicReferenceArray<E>
3.引用类型原子
    AtomicReference<V>
    AtomicStampedReference<V> version号 
    AtomicMarkableReference<V>
        解决是否修改过  mark 是 boolean 类型

4.对象属性修改原子类
    AtomicIntegerFieldUpdater<T>
    AtomicLongFieldUpdater<T>
    AtomicReferenceFieldUpdater<T,V>


    filed 字段必须要public volatile 修饰

5.原子操作增强类
```