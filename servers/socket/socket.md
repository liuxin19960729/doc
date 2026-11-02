# socket
## 网络包
```
1.[用户数据]
2.[AppHeader][用户数据]
3.[TCP Header][AppHeader][用户数据]
socket send
4.[IP Header][TCP Header][AppHeader][用户数据]

5.[Ethernet Header][IP Header][TCP Header][AppHeader][用户数据][Ethernet Footer]
  


```
## samba 服务器安装配置
```
apt install samba -y


/etc/smb.conf
[code]
path=/code
writeable=yes
browseable=yes
guest ok=yes


pkill smbd
smbd

创建code 目录
mkdir /code
给权限
chomd 777 code 


note:
windows 11 需要设置  允许 Guest 匿名访问 和 关闭强制 SMB 签名

Set-SmbClientConfiguration -RequireSecuritySignature $false -Force

Set-SmbClientConfiguration -RequireSecuritySignature $false -Force
```