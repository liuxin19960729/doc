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

不能为空值 不能重复,一个表只能存在一个primary key


CREATE TABLE user(
    id INT USNGINED PRIMARY KEY AUTO_INCREMENT COMMNET '主键注释',
    sex ENUM('男','女') DEFAULT '男' COMMENT '性别' 
)


2.自增约束
auto_increment

3.唯一键
UNIQUE
可以取空值

一个表可以存在多个unique 

4.非空约束
NOT NULL

5.默认值约束
default 

6.外键约束
foreign key


 


desc table ;查看表的描述
desc table \G 查看表的描述

```

## 表设计
```
一对一关系:

父表(主键)-->子表增加一列(关联上父表主键)


一对多关系

一对多和一对一一样子列表需要与之关联父表的主键


多对多关系
    增加一个中间表 
    中间表分别包含多对多关系表的主键



数据库范式带来了那些好处
1.减少了数据库的冗余存储
2.消除异常(插入异常,更新异常,删除异常)
3.数据的组织更加和谐
note:
    (并不是遵循的数据库范式越高越好)范式越高 意味着表越多，多表联合查询的几率越大,查询效率越低


```

## SQL
### 库操作
```sql
show databases; 
use db_name;
create database db_name;
drop database db_name;
```

### 表操作
```sql
show tables;
-- 查看表结构
desc table_name; 
create table table_name(
    .....
);
drop table table_name;


create table user(
   id  int(4) unsigned primary key auto_increment,
   name varchar(50) unique not null,
   age tinyint not null,
   sex enum('M','W') not null
) engine=innodb default charset=utf8;


-- 查看创建表的语句
show create table user;
show create table user \G;



```

### CRUD
```sql
增加
insert into tb_name(col1,col2,col3) values(val1,val2,val3);

insert into user (name,age,sex) values("lx",30,"M");
insert into user (name,age,sex) values("lx1",30,"M");
insert into user (name,age,sex) values("lx2",30,"M");
insert into user (name,age,sex) values("lx3",30,"M");
insert into user (name,age,sex) values("lx4",30,"M");


-- 一次性插入多条数据
insert into user (name,age,sex) values ("lx",30,"M"),("lx1",30,"M"),("lx2",30,"M"),("lx3",30,"M"),("lx4",30,"M");

1.只用一次tcp 连接(三次握手4次挥手)




修改
update tb_name set col1=val1,col2=val2 where id=1;
update tb_name set col1=col1+1;



删除
delete from tb_name where id=1;
delete from tb_name where id in (1,2,3);
delete from tb_name where id between 1 and 10; // >=11 and <=10
// 将表里面的所有数据全部删了
delete from tb_name;


```

### select
```sql

-- 单表查询

select 业务需要哪些字段 写哪些字段（避免*） from tb_name;

select  from tb_name where 条件
         is null
         is not null
         in ()
         not in () 
         between  and
         like -- % _
         > < >= <= 
         and or


select * from user where name like 'l%';




- 将查询出来的内容去重
select distinct  age from user;


union  合并查询

union all 不去重
union 或 union distinct 去重 
例如:SELECT country FROM Websites UNION ALL SELECT country FROM apps ORDER BY
country;


-- 分页查询

select xxx from tb_name limit N
    N  显示前N条数据
select xxx from tb_name limit offset,N
    offset 偏移
select xxx from tb_name limit N offset offset


explain:
    查看sql 的执行计划
    note:explain 查看不了mysql内部对sql做不了优化




create table t_user(
    id int(11) primary key auto_increment,
    email varchar(255) default null,
    password varchar(255) default null
) engine=innodb auto_increment=1000001 default charset=utf8;



where xxx  limit
xxx没有索引的时候 添加 limit 会提高效率,因为mysql 扫描到limit 满足的条数的时候 就停止扫描了



聚合函数
count sum avg max min



分页业务:

pagenum 每页显示的行数

pageno 当前页码
select * from user limit (pageno-1)*pagenum,pagenum;
缺点 :效率不均等 前面查询快 后面查询慢

select *from user where id>上一页数据的最后条 limit pagenum;
不用从头开始扫描 直接从这届从上一页结束的位置往后找pagenum行数据




order by 排序

order by 字段 asc
order by 字段 desc
order by 字段1,字段2,字段3
      默认使用 字段1 排序 等出现相同的值 再使用字段2 排序 以此类推


usring filesort 由于内存大小限制 无法一次性将数据全部加载到内存中 需要使用外排序,需要多次磁盘IO
explain 
Extra:
 Using filesort 外排序

1.排序字段加索引
2.select 查询的字段 看是否回表



group by 分组
select age,count(age)  from user group by age;

将 age 字段值分组 可以使用聚合函数统计具体的值
count sum avg max min


// 对分组后过滤数据
select age,count(age)as num  from user group by age having num>=1;

group by xx,xx 将多个字段分组


group by 涉及排序 explain 查看 extra 里面使用了filesort和temporary 


note: group by 后面使用的字段加索引也是非常有必要的




// 练习
// 总笔数
select count(serno),sum(amount) from bank_bill;


select brno,date,sum(amount) as total_amount from bank_bill group by brno,date order by  brno , total_amount desc;



```
### 多表查询
```sql
内连接查询
    查询是两表交集

外连接查询
    左连接查询
        左表全部显示 右表满足条件的显示 不满足条件的显示为null
    右连接查询
        右表全部显示 左表满足条件的显示 不满足条件的显示为null





student: uid name age sex
course: cid cname credit
exam: uid cid time score


create table student(
    uid int unsigned primary key auto_increment,
    name varchar(50) not null,
    age tinyint unsigned not null,
    sex enum('M','W') not null
);

insert into student (name,age,sex) values
('zhangsan',18,'M'),
('gaoyang',30,'W'),
('chenwei',22,'M'),
('crl',21,'W'),
('crl1',19,'W');


create table course(
    cid int unsigned primary key auto_increment,
    cname varchar(50) not null,
    credit tinyint unsigned not null
);

insert into course (cname,credit) values
('CPP 1',18),
('CPP 2',30),
('CPP PROJECT',22),
('CPP SF',21);

create table exam(
    uid int unsigned not null,
    cid int unsigned not null,
    time date  not null,
    score float unsigned not null,
    primary key (uid,cid)
);

insert into exam (uid,cid,time,score) values
(1,1,'2021-04-09',99.0),
(1,2,'2021-04-09',98.0),
(2,2,'2021-04-09',97.0),
(1,3,'2021-04-09',96.0),
(3,1,'2021-04-09',89.0),
(3,2,'2021-04-09',89.0),
(3,3,'2021-04-09',99.0),
(3,4,'2021-04-09',99.0),
(4,4,'2021-04-09',99.0),
(5,2,'2021-04-09',99.0),
(5,3,'2021-04-09',99.0),
(5,4,'2021-04-09',99.0);


select A.uid,A.name,A.age,A.sex from student as A where A.uid=1;
select C.score from exam as C where C.uid=1 and C.cid=2;


内连接查询
select A.uid,A.name,A.age,A.sex,C.score from student as A inner join exam as C on A.uid=C.uid where C.cid=2 and C.uid=1;

//  on A.uid=C.uid 按记录条数 区分大表和小表
// 小表永远是整表扫描 大表搜索

// 小表student 拿着uid 到大表 搜索 大表创建了 uid和cid 的联合索引


note: 大表建立索引才是最有用的




select b.cid,b.cname,b.credit from course as b



-- exam 关联这 student 和 course  索引 exam 表放在最前面
select a.uid,a.name,b.cname,c.score from exam as c
inner join student as a on a.uid=c.uid
inner join course as b on b.cid=c.cid
where c.cid=2 and c.uid=1;




select b.cname,c.cid,count(c.cid) as num from exam as c
inner join course as b on b.cid=c.cid
group by c.cid 



select b.cname,c.cid,count(c.cid) as num from exam as c
inner join course as b on b.cid=c.cid
group by c.cid having c.cid=2 


select b.cname,c.cid,count(c.cid) as num from exam as c
inner join course as b on b.cid=c.cid
group by c.cid 
order by num desc



单表内连接
select * from tb_name limit offset,limit
select id from tb_name limit offset,limit

一般 id 是索引列 这样查询 性能好于 查询多列字段新能

select a.id..... form tb_name  as a
inner join (select id from tb_name limit offset,limit) as b on a.id=b.id


select id from tb_name limit offset,limit 作为临时表 和 作为内连接小表来扫描 扫描出来的索引 用索引去搜索值 这样就能达到 比较高的效率



note:联合索引 必须使用到第一列才能用到索引

对于inner join 过滤条件卸载where 后面和写在 on里面效果是一样的

-- 外连接查询

left outer join
right outer join


-- left 左边的表全部显示出来 右表中不存在显示数据显示null
select a.*,b.* from student as a left outer join exam as b on a.uid=b.uid;


select a.*,b.* from student as a rights outer join exam as b on a.uid=b.uid;



select * from student where  uid not in (select distinct uid from exam);
(select distinct uid from exam) 会产生一张临时表 ，not in 对索引名命中并不高


外连接 查询条件的时候加载  on 里面 where 里面 只判断对应的空就可以了
写在 on 里面始终保持left 左表全表扫描 ...



```

## 存储引擎
```sql
表结构
表数据
表索引

存储引擎影响上面内容的存储方式


// 查看mysql 支持的存储引擎
show engines;



Innodb(行锁)
.frm 表结构
.idb 数据+索引(如果不设置主键，会自动生成一个隐藏字段作为主键)
MyISAM（表锁）
   .frm 表结构
   .MYD 数据
   .MYI 索引
```
## 索引详解
```sql
note:多列索引必须使用到第一个列才能用到索引


unique 和 primary key 会自动创建索引


索引的创建和应用

create table index1(
    id int,
    name varchar(20),
    sex enum('M','W'),
    INDEX [index_name] (id),// 单列索引
    INDEX  [index_name] (id,name),// 多列索引
)


create [UNIQUE] INDEX index_name ON tb_name (column_list)



drop index index_name on tb_name



```