# maven
[maven](https://maven.apache.org/ref/3.9.16/maven-model/maven.html)
```xml

打包方式
packaging default  jar
    jar war pom

属性配置
properties
   格式
  <name>value</name>.
```
## 创建web工程
```
1.创建一个java se 的maven 工程
2.<packaging>war</packaging> 


note:其实web 和 javase 的区别就是多了一个web模块 src/main/webapp

```
## maven 工程的项目结构
[maven 项目结构](https://maven.apache.org/guides/introduction/introduction-to-the-standard-directory-layout.html)
```
maven 提供了标准的项目结构 

web工程
src/main/webapp/
        index.html web应用的入口文件
        WEB-INF/
            web.xml web应用部署描述文件
            classes/ c存放编译后的class文件
```
## maven的构建
```java
mvn compile 编译项目 生成target文件 note:只能编译核心程序 不能编译测试程序
mvn clean 删除target文件夹

mvn test-compile 编译测试程序 生成target/test-classes文件夹


mvn package 打包 
     将 jar 包 or war 包 打包后放在target目录
mvn install 
将 jar or war 包打包的 本地maven 仓库中
```
## 构建插件 命令 生命周期之间的关系
```java
mvn package
    流水线操作
    mvn compile
    mvn test-compile
    mvn package



周期,命令,周期三者关系
周期->包含若干命令->包含如干插件

```
## 依赖管理
```xml
dependency
    <scope> 标签 依赖范围

scope 默认 compile

compile  test 运行 编译 ,会传递给下游项目
provided 只在编译和测试阶段提供 不会被打包 也不会传递给下游项目 (例如 serlet 你编译的时候需要用到他,但是你在tomcat 容器容器已经提供了它)
runtime 编译的时候不需要 只有在测试和运行的时候需要(例如JDBC驱动 代码是通过Class.forName or 通过SPI加载驱动) 它会传递给下游项目
test 只有在test 阶段使用 
system 
与 provided 类似，但需要显式指定本地文件路径，Maven 不会去仓库查找。由于破坏了可移植性，不推荐使用，应优先将本地 JAR 安装到私服或本地仓库。

<dependency>
    <groupId>com.example</groupId>
    <artifactId>local-lib</artifactId>
    <version>1.0</version>
    <scope>system</scope>
    <systemPath>${project.basedir}/lib/local-lib.jar</systemPath>
</dependency>
import
import
只用于 <dependencyManagement> 中，用来导入另一个 POM 的依赖管理配置（通常是 BOM），本身不引入任何实际依赖。
<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-dependencies</artifactId>
            <version>2.7.0</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
    </dependencies>
</dependencyManagement>
```
## build 配置
```
自定义打包名字
build
    <finalName>自定义名称</finalName>
    生成的文件名称（不包括扩展名，也不包含路径信息）。默认值为${artifactId}-${version}。



resources/resource*
 用于资源路径指定


// 设置打包插件
plugins/plugin*	List<Plugin>	（许多）需要使用的插件列表。


```
## maven 依赖特性
```
jar 包能够传递给下游
scope complie 和 runtime 能顾传递



<dependency> 
    optional 设置为ture 表示不往下传递该依赖(note:即使scope 为complie)

```
## 依赖冲突
```

```