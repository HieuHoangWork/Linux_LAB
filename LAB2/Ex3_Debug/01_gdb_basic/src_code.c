#include <stdio.h>

void hello_print(int i)
{
    printf("Hello %d\n", i);
}

void main()
{
    int i = 0;
    printf("GDB debugger\n");

    for (i = 0; i < 5; i++)
    {
        hello_print(i);
    }
}