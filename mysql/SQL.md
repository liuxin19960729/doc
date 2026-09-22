# SQL
## DCL SQL 
```sql
1.创建用户
CREATE USER 'username'@'host' IDENTIFIED BY 'password';
note: host 为 % 表示所有组件都能够访问


创建用户并指定认证插件
CREATE USER 'username'@'host' IDENTIFIED WITH auth_plugin BY 'password';

2.删除用户
drop USER 'username'@'host'


3.授权
授权所有权限
GRANT ALL PRIVILEGES ON *.* TO 'username'@'host';
授权app_db数据库 select insert update 权限
GRANT SELECT, INSERT, UPDATE ON app_db.* TO 'app_user'@'%';
授权app_db 数据库 users 表 select 权限
GRANT SELECT ON app_db.users TO 'read_only'@'localhost';
撤销app_db 数据库 insert update 权限
REVOKE INSERT, UPDATE ON app_db.* FROM 'app_user'@'%';

REVOKE ALL PRIVILEGES, GRANT OPTION FROM 'username'@'host'; 撤销所有权限


查看指定用户选线
SHOW GRANTS FOR 'username'@'host';
```