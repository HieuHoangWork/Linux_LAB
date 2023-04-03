#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    pid_t child_pid;
    int status;

    child_pid = fork();
    if (child_pid >= 0)
    {
        if (0 == child_pid)
        { /* Process con */
            printf("\nChild process running, child PID: %d\n", getpid());
            printf("My parent PID: %d\n", getppid());
            // printf("Child process terminate after 5 seconds\n");
            // sleep(5);
            // exit(0);

            while (1)
                ;
        }
        else
        { /* Process cha */
            printf("\nParent process running, parent PID:%d\n", getpid());

            /*Trong tiến trình cha, chúng ta sử dụng hàm waitpid với cờ WNOHANG để kiểm tra xem
            tiến trình con có kết thúc hay không mà không cần block tiến trình cha.*/

            if (waitpid(child_pid, &status, 0) == -1)
            {
                perror("waitpid error");
                exit(1);
            }

            if (WIFEXITED(status))
            {
                printf("Normally termination, status = %d\n", WEXITSTATUS(status));
            }
            else if (WIFSIGNALED(status))
            {
                printf("Killed by signal, value = %d\n", WTERMSIG(status));
            }
            while (1)
                ;
        }
    }
    else
    {
        printf("fork() error\n"); // fork() return -1 nếu lỗi.
    }

    return 0;
}