#ifndef __SOCKET_SERVER__
#define __SOCKET_SERVER__

// Lưu trữ thông tin tham số nhập vào khi chạy ./server -> truyền dữ liệu vào thread
typedef struct
{
    char argv_name[30];
} THR_DATA_T;

/* Socket */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
/* Thread */
#include <pthread.h>
/* Semaphore */
#include <semaphore.h>
/* Signal */
#include <sys/wait.h>
#include <signal.h>

#define MAX_CLIENTS 10
void socket_server_processing(void *args);
#endif