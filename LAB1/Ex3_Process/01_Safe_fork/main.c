#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void func(int signum)
{
    printf("Im in func()\n");
    wait(NULL);
}

int main()
{
    pid_t child_pid = fork();

    if (child_pid >= 0)
    {
        if (0 == child_pid)
        {
            printf("\nChild process running, child PID: %d\n", getpid());
            printf("My parent process PID: %d\n", getppid());
            while (1)
                ;
        }
        else
        {
            /**When a child is terminated, a corresponding SIGCHLD signal
             * is delivered to the parent
             */
            signal(SIGCHLD, func);
            printf("\nParent process running, parent PID: %d\n", getpid());
            while (1)
                ;
        }
    }
    else
    {
        printf("fork() unsuccessfully\n");
    }
}