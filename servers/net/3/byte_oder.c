#include <stdio.h>

typedef union byte_oder
{
    short s;
    char c[sizeof(short)];
} un;

int main()
{

    un un = {s : 0x0102};

    if (un.c[0] == 0X01 && un.c[1] == 0X02)
    {
        printf("big\n");
    }
    else if (un.c[0] == 0X02 && un.c[1] == 0X01)
    {
        printf("small\n");
    }
    else
    {
        printf("unkonw\n");
    }

    return 0;
}
