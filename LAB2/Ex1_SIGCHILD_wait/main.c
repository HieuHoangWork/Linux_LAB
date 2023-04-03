#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

void sigchild_handle(int sig)
{
    printf("SIGCHLD handler\n");
}

int main()
{
    signal(SIGCHLD, sigchild_handle);
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
    printf("\nBlocking SIGCHLD");
    sigprocmask(SIG_BLOCK, &mask, NULL);

    pid_t pid = fork();
    if (pid == 0)
    {
        printf("\nChild process exiting\n");
        exit(0);
    }
    else
    {
        int status;
        wait(&status);
        printf("\nParent process unblocking SIGCHLD\n");
        sigprocmask(SIG_UNBLOCK, &mask, NULL);
        sleep(1);
    }
    return 0;
}