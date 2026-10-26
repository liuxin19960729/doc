# crud
## 创建
### 方式1
```golang
err := gorm.G[model.UserModel](db).Create(ctx, &user)
```
### 方式2
```golang

result := gorm.WithResult()
	err := gorm.G[model.UserModel](db, result).Create(ctx, &user)


// 插入多套
err := gorm.G[*model.UserModel](db).CreateInBatches(ctx, &users, len(users))
```
### 方式3
```golang
err := db.Create(&user).Error
```
### 方式4 一次存入多条数据
```golang
	users := []*model.UserModel{
		{
			Name: "crl",
			Age:  18,
			CC:   "email1",
		},
		{
			Name: "crl",
			Age:  18,
			CC:   "email1",
		},
	}

	err := db.Create(&users).Error

```
### 方式5 只存入指定字段的值
```golang
	user :=[] model.UserModel{
		Name: "crl",
		CC:   "email1",
	}

	err := db.Select("Name", "email").Create(&user).Error



note: 也可以存入多条数据
```

### 方式6 忽略指定字段的值
```golang
err := db.Omit("Age").Create(&user).Error
```
