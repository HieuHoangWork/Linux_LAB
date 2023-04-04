#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void SIGUSR1_handle(int signum)
{
    puts("inside catcher!");
}

void check_pending(int signum, char *signame)
{
    sigset_t sigset;

    if (sigpending(&sigset) != 0)
        perror("sigpending() error");
    else if (sigismember(&sigset, signum))
        printf("a %s signal is pending\n", signame);
    else
        printf("no %s signals are pending\n", signame);
}

void main()
{
    sigset_t sigset;
    if (signal(SIGUSR1, SIGUSR1_handle) == SIG_ERR)
        perror("sigaction() error");
    else
    {
        sigemptyset(&sigset);
        sigaddset(&sigset, SIGUSR1);
        if (sigprocmask(SIG_SETMASK, &sigset, NULL) != 0)
            perror("sigprocmask() error");
        else
        {
            puts("SIGUSR1 signals are now blocked");
            kill(getpid(), SIGUSR1);
            printf("after kill: ");
            check_pending(SIGUSR1, "SIGUSR1");
            sigemptyset(&sigset);
            sigprocmask(SIG_SETMASK, &sigset, NULL);
            puts("SIGUSR1 signals are no longer blocked");
            check_pending(SIGUSR1, "SIGUSR1");
        }
    }
}