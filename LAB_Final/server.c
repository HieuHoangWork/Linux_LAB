#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_CLIENTS 10

pthread_t conn_mgr, data_mgr, stor_mgr;
typedef struct
{
    int argc_value;
    char argv_name[30];
} thr_data_t;

static void sigchld_handle(int signum);
static void *thr_conn_mgr_handle(void *args);
static void *thr_data_mgr_handle(void *args);
static void *thr_stor_mgr_handle(void *args);

int main(int argc, char *argv[])
{
    /* Created portnumber on command line */
    if (argc < 2)
    {
        printf("No port provided\ncommand: ./server <port number>\n");
        exit(EXIT_FAILURE);
    }

    /* Created child process */
    pid_t sensor_gateway = fork();

    if (sensor_gateway >= 0)
    {
        if (0 == sensor_gateway)
        {
            int ret;
            /* Created Sensor gateway process - Child process*/
            printf("Sensor gateway process, my PID = %d : parent's PID: %d\n", getpid(), getppid());

            /* Transmit argv[1] ~ port_num to thread conn_mgr  */
            thr_data_t soc_inf;
            memset(&soc_inf, 0x0, sizeof(thr_data_t));

            strncpy(soc_inf.argv_name, argv[1], sizeof(soc_inf.argv_name));

            /* Created 3 threads: conn_mgr, data_mgr, stor_mgr*/
            if (ret = pthread_create(&conn_mgr, NULL, &thr_conn_mgr_handle, &soc_inf))
            {
                printf("pthread_create() error number=%d\n", ret);
                return -1;
            }

            sleep(1);

            if (ret = pthread_create(&data_mgr, NULL, &thr_data_mgr_handle, NULL))
            {
                printf("pthread_create() error number=%d\n", ret);
                return -1;
            }

            sleep(1);

            if (ret = pthread_create(&stor_mgr, NULL, &thr_stor_mgr_handle, NULL))
            {
                printf("pthread_create() error number=%d\n", ret);
                return -1;
            }

            sleep(1);

            /* Thread terminate */
            pthread_join(conn_mgr, NULL);
            pthread_join(data_mgr, NULL);
            pthread_join(stor_mgr, NULL);

            while (1)
                ;
        }
        else
        {
            /* Created Log process - Parent process*/
            sleep(5);
            /* When a child is terminated, a corresponding SIGCHLD signalis delivered to the parent */
            signal(SIGCHLD, sigchld_handle);
            printf("\nLog process, my PID = %d\n", getpid());
            while (1)
                ;
        }
    }
    else
    {
        printf("fork() unsuccessfully\n");
    }
    return 0;
}

static void sigchld_handle(int signum)
{
    printf("Process terminate\n");
    wait(NULL);
}

static void *thr_conn_mgr_handle(void *args)
{
    int server_fd, client_fd[MAX_CLIENTS], max_clients = MAX_CLIENTS, activity, max_sd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    fd_set readfds;
    char buffer[1024];

    thr_data_t *data = (thr_data_t *)args;

    printf("Thread conn_mgr running. Listening on port %s\n", data->argv_name);
    // Initialize all client sockets to 0
    for (int i = 0; i < max_clients; i++)
    {
        client_fd[i] = 0;
    }

    // Create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(atoi(data->argv_name)); /* ./server 8080 */

    // Bind server socket to address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // Clear the socket set
        FD_ZERO(&readfds);

        // Add server socket to set
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        // Add child sockets to set
        for (int i = 0; i < max_clients; i++)
        {
            if (client_fd[i] > 0)
            {
                FD_SET(client_fd[i], &readfds);
            }
            if (client_fd[i] > max_sd)
            {
                max_sd = client_fd[i];
            }
        }

        // Wait for activity on one of the sockets
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0)
        {
            perror("select failed");
            exit(EXIT_FAILURE);
        }

        // If something happened on the server socket,
        // then it is an incoming connection
        if (FD_ISSET(server_fd, &readfds))
        {
            int new_socket;
            if ((new_socket = accept(server_fd,
                                     (struct sockaddr *)&address,
                                     (socklen_t *)&addrlen)) < 0)
            {
                perror("accept failed");
                exit(EXIT_FAILURE);
            }

            // Add new socket to array of sockets
            for (int i = 0; i < max_clients; i++)
            {
                if (client_fd[i] == 0)
                {
                    client_fd[i] = new_socket;
                    break;
                }
            }
        }

        // Else it is some IO operation on some other socket
        for (int i = 0; i < max_clients; i++)
        {
            if (FD_ISSET(client_fd[i], &readfds))
            {
                int valread;
                if ((valread = read(client_fd[i], buffer, 1024)) == 0)
                {
                    // Somebody disconnected
                    close(client_fd[i]);
                    client_fd[i] = 0;
                }
                else
                {
                    buffer[valread] = '\0';
                    printf("%s\n", buffer);
                }
            }
        }
    }
}

static void *thr_data_mgr_handle(void *args)
{
    printf("Thread data_mgr running ...\n");
}

static void *thr_stor_mgr_handle(void *args)
{
    printf("Thread stor_mgr running ...\n");
}