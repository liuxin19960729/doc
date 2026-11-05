# struct
```c

基本定义语法


struct 结构体名称
{
    数据类型 成员名称 : 位数;
};



例如

#include <stdio.h>


// 实际就是一个字节大小
struct Data
{
    unsigned char a : 1;
    unsigned char b : 2;
    unsigned char c : 1;
};

int main()
{

    struct Data data;

    data.a = 1;
    data.b = 3;
    data.c = 1;
    // 0000 1111
    printf("a:%d\n", *(char *)(&data)); //15
}


```