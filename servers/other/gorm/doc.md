# gorm
## 安装
```go
go get -u  gorm.io/gorm

go get -u gorm.io/driver/mysql
```
## 连接Mysql
[更多详情](https://gorm.io/zh_CN/docs/connecting_to_the_database.html)
```golang


import (
  "gorm.io/driver/mysql"
  "gorm.io/gorm"
)

func main() {
  // 参考 https://github.com/go-sql-driver/mysql#dsn-data-source-name 获取详情
  dsn := "user:pass@tcp(127.0.0.1:3306)/dbname?charset=utf8mb4&parseTime=True"
  db, err := gorm.Open(mysql.Open(dsn), &gorm.Config{})
}



note:
    想要正确的处理time.Time 需要带上 parseTime=Tru
    要支持完整的 UTF-8 编码 charset=utf8mb4


```

## 模型定义
[更多详情](https://gorm.io/zh_CN/docs/models.html)

### 约定
```
1.主键
GORM 使用一个名为ID 的字段作为每个模型的默认主键


2.表名
   默认情况下Gorm将结构体的名字转换为表明+负数的形式

   例如 GormUserName  表名转为 gorm_user_names


3.列名
    转为小写方式

4.时间戳字段
    GORM使用字段 CreatedAt 和 UpdatedAt 来自动跟踪记录的创建和更新时间。


```
### gorm.Model
```golang

// gorm.Model 的定义
type Model struct {
  ID        uint           `gorm:"primaryKey"`
  CreatedAt time.Time
  UpdatedAt time.Time
  DeletedAt gorm.DeletedAt `gorm:"index"`
}



如果你需要这些字段可以将 gorm.Model 嵌入你自己的结构体


```

### 高级选型
#### 字段级权限控制
```
<-:create
<-update
<- 允许读和写(创建+更新)
<-:false 允许读 禁止写

-> 只读
->;<-:creat 读和写

->:false;<-:create  只能创建 不允许读



gorm:"-"
gorm:"-:all" 
    仅用作程序处理不映射到数据库
    所有的orm 操作都被忽略
gorm:"-:migration"
    这个字段不参与数据库结构迁移，但仍然可以参与正常的数据库读写操作
    该自断不能通过gorm 创建只能由SQL 脚本手动管理
```
#### 创建/更新时间追踪（纳秒、毫秒、秒、Time）
```
CreatedAt UpdatedAt 约定创建和更新自动填充时间 
如果其他字段也想自动填充时间 



类型 int64  保存的是时间戳
如果要保存 时间则使用time.Time

`gorm:"autoUpdateTime:nano"`
`gorm:"autoUpdateTime:milli"`
`gorm:"autoCreateTime"`



```

#### 嵌入结构体
```golang
1.匿名字段嵌入


2.gorm:"embedded"`
    例如

type Author struct {
    Name  string
    Email string
}

type Blog struct {
  ID      int
  Author  Author `gorm:"embedded"`
 



 3.`gorm:"embedded;embeddedPrefix:author_"`

    embeddedPrefix:author_ 给嵌入的字段添加前缀
    
```

#### 字段标签
[link](https://gorm.io/zh_CN/docs/models.html#%E5%AD%97%E6%AE%B5%E6%A0%87%E7%AD%BE)

## 迁移
[link](https://gorm.io/zh_CN/docs/migration.html)
```
note:
    AutoMigrate 创建表 缺失外键 约束 和 列 索引 但是为了保证数据的安全不会删除未使用的类


note:
 AutoMigrate 会自动创建数据库外键约束，您可以在初始化时禁用此功能，例如
 db, err := gorm.Open(sqlite.Open("gorm.db"), &gorm.Config{
  DisableForeignKeyConstraintWhenMigrating: true,
})



CurrentDatabase 获取当前数据库名称


// 将 "ENGINE=InnoDB" 添加到创建 `User` 的 SQL 里去
db.Set("gorm:table_options", "ENGINE=InnoDB").Migrator().CreateTable(&User{})

HasTable 是否存在该表
DropTable 删除该表
RenameTable 修改该表的名称


```

