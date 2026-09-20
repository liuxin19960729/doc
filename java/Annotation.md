# Annotation
```java
注解位置
1.类
2.构造器
3.方法
4.成员变量
5.参数
.....

public @interface MyAnnotation {
    属性类型 属性名() default 默认值
}




note:
   如果注解中只有一个属性 并且该属性名字为value 使用注解的时候可以不用写属性名 

例:
1.
public @interface AnnT1 {
    String value() default "unkow";
}


@AnnT1("TestAnnT1")
public class TestAnnT1 {
    public static void main(String[] args) {

    }
}


2.
public @interface AnnT1 {
    String value() default "unkow";

    int age() default 18;
}

@AnnT1("TestAnnT1") // age 使用默认值为18
public class TestAnnT1 {
    public static void main(String[] args) {

    }
}




注解反编译后的源码

public interface org.example.d5.AnnT1 extends java.lang.annotation.Annotation {
 // 抽象方法
  public abstract java.lang.String value();
}

@AnnT1("TestAnnT1")  实际是创建一个该注解的实现类对象


```
## 元注解
```java
修饰注解的的注解


元注解	作用
@Target	指定注解可以用在哪些地方
    TYPE           // 类、接口、枚举
    FIELD          // 字段
    METHOD         // 方法
    PARAMETER      // 参数
    CONSTRUCTOR    // 构造器
    LOCAL_VARIABLE // 局部变量
    ANNOTATION_TYPE// 注解类型
    PACKAGE        // 包
    TYPE_PARAMETER // 类型参数 (Java 8+)
    TYPE_USE       // 类型使用 (Java 8+)
@Retention	指定注解的保留策略
    SOURCE   // 仅源码，编译后丢弃（如 @Override）
    CLASS    // 保留到 class 文件，但运行时不可见（默认）
    RUNTIME  // 保留到运行时，可通过反射读取

@Documented	是否包含在 Javadoc 中
@Inherited	子类是否继承父类的注解
@Repeatable	是否可重复使用

```

## 解析注解
```java
public interface AnnotatedElement


Class Method Filed Constructor 都实现了AnnotatedElement ,他们都拥有解析注解的能力



default boolean isAnnotationPresent(Class<? extends Annotation> annotationClass)
是否有该注解
```