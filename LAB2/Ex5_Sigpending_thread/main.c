#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

pthread_t thread_id1, thread_id2;

void SIGUSR1_handle(int signum)
{
    puts("Signal user 1 running");
}

void SIGUSR2_handle(int signum)
{
    puts("Signal user 2 running");
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

static void *thr_handle1()
{
    sigset_t sigset;
    pthread_t tid = pthread_self();
    if (pthread_equal(tid, thread_id1))
    {
        printf("I'm thread_id1\n");
    }

    signal(SIGUSR1, SIGUSR1_handle);
    signal(SIGUSR2, SIGUSR2_handle);
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);
    sigprocmask(SIG_SETMASK, &sigset, NULL);
    puts("SIGUSR1 signals are now blocked in thread 1");
    pthread_kill(tid, SIGUSR1);
    pthread_kill(tid, SIGUSR2);
    check_pending(SIGUSR1, "SIGUSR1");
    check_pending(SIGUSR2, "SIGUSR2");
}

static void *thr_handle2()
{
    sigset_t sigset;
    pthread_t tid = pthread_self();
    if (pthread_equal(tid, thread_id2))
    {
        printf("I'm thread_id2\n");
    }

    signal(SIGUSR1, SIGUSR1_handle);
    signal(SIGUSR2, SIGUSR2_handle);
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR2);
    sigprocmask(SIG_SETMASK, &sigset, NULL);
    puts("SIGUSR2 signals are now blocked in thread 2");
    pthread_kill(tid, SIGUSR1);
    pthread_kill(tid, SIGUSR2);
    check_pending(SIGUSR1, "SIGUSR1");
    check_pending(SIGUSR2, "SIGUSR2");
}

int main()
{
    int ret;
    if (ret = pthread_create(&thread_id1, NULL, &thr_handle1, NULL))
    {
        printf("pthread_create() error number=%d\n", ret);
        return -1;
    }

    sleep(5);
    printf("___________________________________\n");

    if (ret = pthread_create(&thread_id2, NULL, &thr_handle2, NULL))
    {
        printf("pthread_create() error number=%d\n", ret);
        return -1;
    }

    pthread_join(thread_id1, NULL);
    pthread_join(thread_id2, NULL);

    return 0;
}