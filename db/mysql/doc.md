# mysql
## 下载安装 Mysql5.7
[download](https://downloads.mysql.com/archives/community/)
[install](https://docs.oracle.com/cd/E17952_01/mysql-5.7-en/linux-installation-debian.html?utm_source=chatgpt.com)
```
配置文件 /etc/mysql
数据目录在/var/lib/mysql

```

## 设置支持ip远程连接
```sql
use mysql;

-- 查看用户支持的连接ip
select Host,User,authentication_string from user;

-- 给 root用户授权并且支持任何ip连接
grant all privileges on *.* to 'root'@'%' identified by '密码' with
grant option;

flush privileges;


note:
   如果还是连接不上到配置文件检测 bind-address 是否为 0.0.0.0
   修改好了并且重启mysql 服务



```
## 数据类型
### 整数类型
```sql

tinyint (byte)
smallint  (2byte)
mediumint (3 byte)
int   (4byte)
bigint (8byte)

unsigned 无符号

浮点数
float (4byte)
double (8byte)

浮点数类型推荐
decimal 类型(保存为字符串格式)  



int(9):
    int 大小4个字节是固定的,这个9表示显示宽度为9
    例如 值为 123 显示的时候 000 000 123 这样显示
```
### 整数串类型
```sql
char (固定长度 M(0-255))
   char(12) 存储 hello 存储在磁盘大小 还是12byte

varchar (可变长度 M(0-65535))
    varchar(12) 存储 hello 存储在磁盘大小是实际字符串大小




二进制 数据 例如图片 .....

tinyblob  长度 +1byte
bolob   长度 +2byte
mediumblob   长度 +3byte
longblob    长度 +4byte



tinytext 长度 +1byte
text 长度 +2byte
mediumtext 长度 +3byte
longtext 长度 +4byte

varbinary(m) (0-255 变长字节)
binary(m) (0-255 固定长度)

```

### 时间类型
```sql
DATE 4 byte 1000-01-01 9999-12-31
DATETIME 8 byte 1000-01-01 00:00:00    9999-12-31 23:59:59
TIMESTAMP 4 byte 19700101080001 2038年的某一时刻
TIME 3 byte -838:59:59 838:59:59
YEAR 1 byte 1901-2155


-- note 一般在mysql数据库存储时间戳
select unix_timestamp(now());


```
### enum和 set
```
enum 显示 固定的值

set 显示只能存在一个值

```

## Mysql 运算符
```sql
+ - * /(DIV) %(MOD)

! ||(or) && (and)


=!=  > <  >= <=  

xx between a and b 在范围内 相等于 value >= a AND value <= b
in (a,b,c)在集合内 
is null
is not null

like 通配符
   例如 like "zhang%"
        like "zhang_"

        note % 匹配多个字符
             _ 匹配多个字符



```

## Mysql 完整性约束
```sql

1.主键约束

primary key

不能为空值 不能重复,一个表只能存在一个组件


CREATE TABLE user(
    id INT USNGINED PRIMARY KEY AUTO_INCREMENT COMMNET '主键注释',
    sex ENUM('男','女') DEFAULT '男' COMMENT '性别' 
)


2.自增约束
auto_increment

3.唯一键
UNIQUE
可以取空值


4.非空约束
NOT NULL

5.默认值约束
default 

6.外键约束
foreign key


 


desc table ;查看表的描述
desc table \G 查看表的描述

```