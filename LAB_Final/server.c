#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <pthread.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 2048

/* Hàm xử lý lỗi */
#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

/* Initizalize 3 threads: conn_mgr, data_mgr, stor_mgr */
pthread_t conn_mgr, data_mgr, stor_mgr;

/* Handle synchronization data in thread data_mgr and stor_mgr */
pthread_cond_t data_ready_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t data_ready_mutex = PTHREAD_MUTEX_INITIALIZER;
int data_ready = 0;

/* Lưu trữ thông tin tham số nhập vào khi chạy ./server -> truyền dữ liệu vào thread */
typedef struct
{
    int argc_value;
    char argv_name[30];
} THR_DATA_T;

/* Link list data receive and synchronization */
typedef struct SensorData
{
    char sensor_id[32];
    float temperature;
    time_t timestamp;
    struct SensorData *next;
} SensorData;

pthread_mutex_t data_mutex;
SensorData *head = NULL;

/* Function handle data */
static void cleanup();
static void sigchld_handle(int signum);
static void *thr_conn_mgr_handle(void *args);
static void *thr_data_mgr_handle(void *args);
static void *thr_stor_mgr_handle(void *args);

/* Main */
int main(int argc, char *argv[])
{
    /* Created portnumber on command line */
    if (argc != 2)
    {
        printf("No port provided\ncommand: ./server <port number>\n");
        exit(EXIT_FAILURE);
    }

    /* Init mutex */
    pthread_mutex_init(&data_mutex, NULL);

    /* Created child process */
    pid_t sensor_gateway = fork();

    if (sensor_gateway >= 0)
    {
        if (0 == sensor_gateway)
        {
            /* Created Sensor gateway process - Child process*/
            printf("Sensor gateway process, my PID = %d : parent's PID: %d\n", getpid(), getppid());

            /* Transmit argv[1] = port_num to thread conn_mgr  */
            THR_DATA_T thr_data;
            memset(&thr_data, 0x0, sizeof(THR_DATA_T));
            strncpy(thr_data.argv_name, argv[1], sizeof(thr_data.argv_name));

            /* Created 3 threads: conn_mgr, data_mgr, stor_mgr*/
            pthread_create(&conn_mgr, NULL, &thr_conn_mgr_handle, &thr_data);
            pthread_create(&data_mgr, NULL, &thr_data_mgr_handle, NULL);
            pthread_create(&stor_mgr, NULL, &thr_stor_mgr_handle, NULL);

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
            sleep(3);
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
static void cleanup()
{
    pthread_mutex_destroy(&data_mutex);
    pthread_mutex_destroy(&data_ready_mutex);
    pthread_cond_destroy(&data_ready_cond);

    SensorData *current = head;
    SensorData *temp;

    while (current != NULL)
    {
        temp = current;
        current = current->next;
        free(temp);
    }
}

/* Hàm waitpid sẽ chờ đợi cho tất cả các process con đã kết thúc */
static void sigchld_handle(int signum)
{
    printf("Process terminate\n");
    cleanup();
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
}

static void *thr_conn_mgr_handle(void *args)
{
    THR_DATA_T *data = (THR_DATA_T *)args;
    int server_fd, client_fd, max_clients = MAX_CLIENTS, activity, max_sd;
    struct sockaddr_in server_address, client_address;
    socklen_t addrlen = sizeof(client_address);
    fd_set readfds;
    char buffer[BUFFER_SIZE];

    /* Khởi tạo địa chỉ server */
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(atoi(data->argv_name));

    /* Tạo socket */
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

    while (1)
    {
        // Clear the socket set
        FD_ZERO(&readfds);

        // Add server socket to set
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        // Wait for activity on one of the sockets
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0)
        {
            handle_error("select failed");
        }

        // If something happened on the server socket, it is an incoming connection
        if (FD_ISSET(server_fd, &readfds))
        {
            if ((client_fd = accept(server_fd, (struct sockaddr *)&client_address, &addrlen)) < 0)
            {
                handle_error("accept failed");
            }
            printf("New client connected, IP: %s, Port: %d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

            int disconnected = 0;
            // Else it is some IO operation on some other socket
            while (!disconnected) // Thay đổi từ vòng lặp vô hạn sang kiểm tra cờ disconnected
            {
                int valread;

                if ((valread = read(client_fd, buffer, BUFFER_SIZE)) == 0)
                {
                    // Somebody disconnected
                    printf("Device disconnected, IP: %s, Port: %d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                    close(client_fd);
                    disconnected = 1; // Đặt cờ disconnected khi kết nối bị ngắt kết nối
                }
                else
                {
                    buffer[valread] = '\0';
                    printf("Received from client: %s\n", buffer);

                    // Phân tích dữ liệu
                    char sensor_id[32];
                    float temperature;
                    time_t timestamp;

                    sscanf(buffer, "/%31[^/]/%f/%ld", sensor_id, &temperature, &timestamp);

                    // Tạo một node mới và thêm vào danh sách liên kết
                    SensorData *new_node = (SensorData *)malloc(sizeof(SensorData));
                    strcpy(new_node->sensor_id, sensor_id);
                    new_node->temperature = temperature;
                    new_node->timestamp = timestamp;
                    new_node->next = NULL;

                    pthread_mutex_lock(&data_mutex);
                    new_node->next = head;
                    head = new_node;
                    pthread_mutex_unlock(&data_mutex);

                    pthread_mutex_lock(&data_ready_mutex);
                    data_ready = 1;
                    pthread_cond_signal(&data_ready_cond);
                    pthread_mutex_unlock(&data_ready_mutex);
                }
            }
            disconnected = 0; // Đặt lại cờ disconnected để chờ kết nối tiếp theo
        }
    }
    return NULL;
}

static void *thr_data_mgr_handle(void *args)
{
    printf("Thread data_mgr running ...\n");

    while (1)
    {
        pthread_mutex_lock(&data_ready_mutex);
        while (!data_ready)
        {
            pthread_cond_wait(&data_ready_cond, &data_ready_mutex);
        }
        data_ready = 0;
        pthread_mutex_unlock(&data_ready_mutex);

        pthread_mutex_lock(&data_mutex);
        SensorData *current = head;
        SensorData *previous = NULL;
        int count = 0;
        float sum_temperature = 0;
        time_t sum_timestamp = 0;
        char sensor_id[32];

        while (current != NULL)
        {
            if (count == 0)
            {
                strcpy(sensor_id, current->sensor_id);
            }

            if (strcmp(sensor_id, current->sensor_id) == 0)
            {
                sum_temperature += current->temperature;
                sum_timestamp += current->timestamp;
                count++;

                if (count == 5)
                {
                    float average_temperature = sum_temperature / count;
                    time_t average_timestamp = sum_timestamp / count;
                    const char *temp_string = average_temperature > 20 ? "TOO HOT" : (average_temperature < 13 ? "TOO COLD" : "NORMAL");

                    printf("/%s/%ld/%s\n", sensor_id, average_timestamp, temp_string);

                    // Send signal to stor_mgr
                    pthread_mutex_lock(&data_ready_mutex);
                    data_ready = 1;
                    pthread_cond_signal(&data_ready_cond);
                    pthread_mutex_unlock(&data_ready_mutex);
                    break;
                }
            }

            previous = current;
            current = current->next;
        }
        pthread_mutex_unlock(&data_mutex);
    }

    return NULL;
}

static void *thr_stor_mgr_handle(void *args)
{
    printf("Thread stor_mgr running ...\n");

    while (1)
    {
        pthread_mutex_lock(&data_ready_mutex);
        while (!data_ready)
        {
            pthread_cond_wait(&data_ready_cond, &data_ready_mutex);
        }
        data_ready = 0;
        pthread_mutex_unlock(&data_ready_mutex);

        pthread_mutex_lock(&data_mutex);
        SensorData *current = head;
        SensorData *previous = NULL;
        FILE *file = fopen("Data.txt", "a");

        int count = 0;
        char sensor_id[32];

        while (current != NULL && count < 5)
        {
            if (count == 0)
            {
                strcpy(sensor_id, current->sensor_id);
            }

            if (strcmp(sensor_id, current->sensor_id) == 0)
            {
                fprintf(file, "/%s/%f/%ld\n", current->sensor_id, current->temperature, current->timestamp);
                head = current->next;
                free(current);
                current = head;
                count++;
            }
            else
            {
                previous = current;
                current = current->next;
            }
        }
        fclose(file);
        pthread_mutex_unlock(&data_mutex);
    }

    return NULL;
}