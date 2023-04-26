#include "processing_linked_list.h"
#include "processing_FIFO.h"
#include "socket_server.h"
#include <errno.h>

// Initizalize 3 threads: conn_mgr, data_mgr, stor_mgr
pthread_t conn_mgr, data_mgr, stor_mgr;

// Synchronized data
pthread_mutex_t data_mutex;
sem_t ser_ready_for_data_mgr;
sem_t ser_ready_for_stor_mgr;

// Khởi tạo danh sách liên kết
Node *head = NULL;

// Lưu trữ trạng thái của chương trình
volatile sig_atomic_t running = 1;

// Khởi tạo số sequence number cho FIFO
unsigned int log_sequence_number = 0;

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

    // Created FIFO
    created_FIFO(FIFO_PATH);

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

            while (running)
            {
                write_to_LOG_file(FIFO_PATH, LOG_PATH);
            }
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
    socket_server_processing(args);
    return NULL;
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
        sort_linked_list_by_ID_and_TIME(&head);
        averaged_temp_linked_list(head);
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
        copy_data_from_linked_list_to_data_file(head);
        // Xoa du lieu da xu ly
        remove_data_from_linked_list(&head);
        pthread_mutex_unlock(&data_mutex);
    }
    return NULL;
}
