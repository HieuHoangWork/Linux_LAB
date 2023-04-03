#include <stdio.h>
#include <stdlib.h>

void foo()
{
    int *p = NULL;
    printf("*p = %d\n", *p);
}

void main()
{
    printf("Demo core dump\n");
    foo();
}
