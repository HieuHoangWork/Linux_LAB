#include <stdlib.h>
#include <stdio.h>

int main()
{
    int return_value;
    return_value = system("ls -l");
    printf("Return value: %d\n", return_value);
    return 0;
}