#include <stdio.h>
#include <stdlib.h>
#include "gpio.h"

int option;

void printMenu()
{

    printf("**** Please Enter the Option *****\n");
    printf("        1.  LED on                \n");
    printf("        0.  LED off               \n");
    printf("        99. Exit                  \n");
    printf("**********************************\n");
    printf(">>> ");
}

int main()
{
    printf("*********************************\n");
    printf("******* Linux From Scratch *******\n\n");
    while (1)
    {
        printMenu();

        scanf("%d", &option);
        switch (option)
        {
        case 1:
            printf("Turn LED on\n");
            GPIO_Direction(OUT);
            GPIO_Value(HIGH);
            printf("Done!\n\n\n");
            break;

        case 0:
            printf("Turn LED off\n");
            GPIO_Direction(IN);
            GPIO_Value(LOW);
            break;
        case 99:
            exit(1);
            break;
        default:
            printf("Enter invalid option: %c\n", option);
            break;
        }
    }
    return 0;
}