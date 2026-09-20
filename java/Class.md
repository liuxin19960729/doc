# Class
## 获取类的构造器
```java
public Constructor<?>[] getConstructors() 
获取Public 的所有构造器

public Constructor<?>[] getDeclaredConstructors()
获取 所有构造器


public Constructor<T> getConstructor(Class<?>... parameterTypes) throws NoSuchMethodException
public Constructor<T> getDeclaredConstructor(Class<?>... parameterTypes) throws NoSuchMethodException


```
## 获取类构造器的作用
```java

public void setAccessible(boolean flag)
 设置为true,被反射对象忽略Java语言访问控制的检查



public T newInstance(Object... initargs)
              throws InstantiationException,
              IllegalAccessException,
              IllegalArgumentException,
              InvocationTargetException


```
## 获取类的成员变量
```java
public Field[] getFields()
public Field[] getDeclaredFields()
public Field getField(String name) throws NoSuchFieldException
public Field getDeclaredField(String name) throws NoSuchFieldException



// 获取设置成员变量的值
public void setAccessible(boolean flag)
 设置为true,被反射对象忽略Java语言访问控制的检查
public Object get(Object obj) throws IllegalArgumentException,IllegalAccessException
public void set(Object obj,Object value) throws IllegalArgumentException,IllegalAccessException
```
## 获取类的成员方法
```java
public Method[] getMethods()
public Method[] getDeclaredMethods()
public Method getMethod(String name,Class<?>... parameterTypes) sthrows NoSuchMethodException
public Method getDeclaredMethod(String name,Class<?>... parameterTypes) throws NoSuchMethodException


//Method 对象函数
public void setAccessible(boolean flag)

public Object invoke(Object obj,Object... args) throws IllegalAccessException, InvocationTargetException
```