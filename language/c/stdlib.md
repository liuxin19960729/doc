# stdlib
## getenv
```c


char *getenv(const char *name);
获取当前进程环境变量的值


return
  char * 指向环境变量的值的指针
  NULL 环境变量不存在
```

## 字符串转换数字
```c
int atoi(const char *str);
long atol(const char *nptr);
long long atoll(const char *nptr);
将字符串转换为整数


return
    返回有效的整数值
    0， M诶呦有效的整数值



long strtol(const char *nptr,char **endptr,int base);
long long strtoll(const char *restrict nptr,
                   char **_Nullable restrict endptr, int base);


nptr 待转换的字符串
endptr 转换停止的位置
base 进制 10 16 8



```
