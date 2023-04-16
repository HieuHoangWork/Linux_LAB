#include "handle_data.h"
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h> /* Socket */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>   /* Thread */
#include <semaphore.h> /* Semaphore */
#include <sys/wait.h>  /* Signal */
#include <signal.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 2048

// Hàm xử lý lỗi
#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

// Initizalize 3 threads: conn_mgr, data_mgr, stor_mgr
pthread_t conn_mgr, data_mgr, stor_mgr;

// Synchronized data
pthread_mutex_t data_mutex;
sem_t ser_ready_for_data_mgr;
sem_t ser_ready_for_stor_mgr;

// Lưu trữ thông tin tham số nhập vào khi chạy ./server -> truyền dữ liệu vào thread
typedef struct
{
    char argv_name[30];
} THR_DATA_T;

// Khởi tạo danh sách liên kết
Node *head = NULL;

// Lưu trữ trạng thái của chương trình
volatile sig_atomic_t running = 1;

// Function handle data
static void cleanup();
static void sigterm_handle(int signum);
static void *thr_conn_mgr_handle(void *args);
static void *thr_data_mgr_handle(void *args);
static void *thr_stor_mgr_handle(void *args);

// Main
int main(int argc, char *argv[])
{
    // Created portnumber on command line
    if (argc != 2)
    {
        printf("No port provided\ncommand: ./server <port number>\n");
        exit(EXIT_FAILURE);
    }

    // Init Synchronized data
    pthread_mutex_init(&data_mutex, NULL);
    sem_init(&ser_ready_for_data_mgr, 0, 0);
    sem_init(&ser_ready_for_stor_mgr, 0, 0);

    // Created child process
    pid_t sensor_gateway = fork();

    if (sensor_gateway >= 0)
    {
        if (0 == sensor_gateway)
        {
            // Created Sensor gateway process - Child process
            printf("Sensor gateway process, my PID = %d : parent's PID: %d\n", getpid(), getppid());

            // Transmit argv[1] = port_num to thread conn_mgr
            THR_DATA_T thr_data;
            memset(&thr_data, 0x0, sizeof(THR_DATA_T));
            strncpy(thr_data.argv_name, argv[1], sizeof(thr_data.argv_name));

            // Created 3 threads: conn_mgr, data_mgr, stor_mgr
            pthread_create(&conn_mgr, NULL, &thr_conn_mgr_handle, &thr_data);
            pthread_create(&data_mgr, NULL, &thr_data_mgr_handle, NULL);
            pthread_create(&stor_mgr, NULL, &thr_stor_mgr_handle, NULL);

            // Thread terminate
            pthread_join(conn_mgr, NULL);

            // Kết thúc các thread khác
            pthread_cancel(data_mgr);
            pthread_join(data_mgr, NULL);
            pthread_cancel(stor_mgr);
            pthread_join(stor_mgr, NULL);

            // Gửi tín hiệu kết thúc cho process cha
            kill(getppid(), SIGTERM);

            while (1)
                ;
        }
        else
        {
            // Created Log process - Parent process
            sleep(1);
            // When a child is terminated, a corresponding SIGCHLD signalis delivered to the parent
            signal(SIGTERM, sigterm_handle);
            printf("\nLog process, my PID = %d\n", getpid());
            while (1)
                ;
        }
    }
    else
    {
        // Fork không thành công
        handle_error("fork");
    }

    // Clean up
    cleanup();

    return 0;
}
static void cleanup()
{
    free_linked_list(head);
    pthread_mutex_destroy(&data_mutex);
    sem_destroy(&ser_ready_for_data_mgr);
    sem_destroy(&ser_ready_for_stor_mgr);
}

// Hàm waitpid sẽ chờ đợi cho tất cả các process con đã kết thúc
static void sigterm_handle(int signum)
{
    // Xử lý tín hiệu SIGTERM
    printf("Log process received SIGTERM, shutting down...\n");
    cleanup();
    exit(EXIT_SUCCESS);
}

static void *thr_conn_mgr_handle(void *args)
{
    THR_DATA_T *data = (THR_DATA_T *)args;
    int server_fd, client_fd, max_clients = MAX_CLIENTS, activity, max_sd, sd;
    struct sockaddr_in server_address, client_address;
    socklen_t addrlen = sizeof(client_address);
    fd_set readfds;

    // Mảng chứa tất cả các client socket
    int client_socket[MAX_CLIENTS];
    for (int i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }

    // Khởi tạo địa chỉ server
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(atoi(data->argv_name));

    // Tạo socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
        handle_error("socket()");

    // Bind server socket to address
    if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        handle_error("bind failed");
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        handle_error("listen failed");
    }

    printf("Thread conn_mgr running. Listening on port %s\n", data->argv_name);

    while (running)
    {
        // Clear the socket set
        FD_ZERO(&readfds);

        // Add server socket to set
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        // Add child sockets to set
        for (int i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];

            if (sd > 0)
                FD_SET(sd, &readfds);

            if (sd > max_sd)
                max_sd = sd;
        }

        // Wait for activity on one of the sockets
        struct timeval timeout;
        timeout.tv_sec = 7;
        timeout.tv_usec = 0;

        activity = select(max_sd + 1, &readfds, NULL, NULL, &timeout);

        if (activity < 0)
        {
            handle_error("select failed");
        }

        if (activity == 0)
        {
            printf("No activity within 7 seconds. Exiting...\n");
            running = 0; // Cập nhật trạng thái của chương trình
            break;
        }

        // If something happened on the server socket, it is an incoming connection
        if (FD_ISSET(server_fd, &readfds))
        {
            if ((client_fd = accept(server_fd, (struct sockaddr *)&client_address, &addrlen)) < 0)
            {
                handle_error("accept failed");
            }
            printf("New client connected, IP: %s, Port: %d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

            // Add new socket to array of sockets
            for (int i = 0; i < max_clients; i++)
            {
                if (client_socket[i] == 0)
                {
                    client_socket[i] = client_fd;
                    break;
                }
            }
        }
        else
        {
            // Else it is some IO operation on some other socket
            for (int i = 0; i < max_clients; i++)
            {
                sd = client_socket[i];
                if (FD_ISSET(sd, &readfds))
                {
                    int valread;
                    char buffer[BUFFER_SIZE];

                    if ((valread = read(sd, buffer, BUFFER_SIZE)) == 0)
                    {
                        // Somebody disconnected
                        getpeername(sd, (struct sockaddr *)&client_address, &addrlen);
                        printf("Device disconnected, IP: %s, Port: %d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                        close(sd);
                        client_socket[i] = 0;
                    }
                    else
                    {
                        buffer[valread] = '\0';
                        printf("Received from client: %s\n", buffer);

                        // Phân tích dữ liệu
                        char sensor_id[32];
                        float temperature;
                        time_t timestamp;

                        sscanf(buffer, "%31[^/]/%f/%ld", sensor_id, &temperature, &timestamp);

                        // Tạo một node mới và thêm vào danh sách liên kết
                        pthread_mutex_lock(&data_mutex);
                        data_save(&head, atoi(sensor_id), temperature, timestamp);
                        pthread_mutex_unlock(&data_mutex);

                        // Gửi tín hiệu đến thr_data_mgr_handle sau khi thêm một node mới
                        sem_post(&ser_ready_for_data_mgr);
                    }
                }
            }
        }
    }
    // Close all the sockets
    for (int i = 0; i < max_clients; i++)
    {
        if (client_socket[i] != 0)
        {
            close(client_socket[i]);
        }
    }
    close(server_fd);

    pthread_exit(NULL);
}

static void *thr_data_mgr_handle(void *args)
{
    printf("Thread data_mgr running ...\n");
    while (running)
    {
        // Chờ tín hiệu từ semaphore
        sem_wait(&ser_ready_for_data_mgr);

        // Xử lý dữ liệu khi nhận được tín hiệu
        pthread_mutex_lock(&data_mutex);
        sort_list_by_ID(&head);
        data_handle(head);
        pthread_mutex_unlock(&data_mutex);

        // Gửi tín hiệu đến thr_stor_mgr_handle sau khi đã xử lý
        sem_post(&ser_ready_for_stor_mgr);
    }
    return NULL;
}

static void *thr_stor_mgr_handle(void *args)
{
    printf("Thread stor_mgr running ...\n");
    while (running)
    {
        // Chờ tín hiệu từ semaphore
        sem_wait(&ser_ready_for_stor_mgr);

        pthread_mutex_lock(&data_mutex);
        // Save data da xu ly
        save_data_to_file(head);
        // Xoa du lieu da xu ly
        remove_data(&head);
        pthread_mutex_unlock(&data_mutex);
    }
    return NULL;
}