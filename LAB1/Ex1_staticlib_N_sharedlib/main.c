#include <stdio.h>
#include "Addition.h"
#include "Subtraction.h"

int a = 20, b = 10;

int main()
{
    printf("Starting\n");
    addition(&a, &b);
    subtraction(&a, &b);
}
