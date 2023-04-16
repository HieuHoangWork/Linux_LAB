#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>

#define BUFFER_SIZE 2048
#define DEVICE_ID "1"

/* Hàm xử lý lỗi */
#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: ./client <hostname> <port number>\n");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    int server_fd;
    struct sockaddr_in serv_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));

    /* Khởi tạo địa chỉ server */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) == -1)
        handle_error("inet_pton()");

    /* Tạo socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
        handle_error("socket()");

    /* Kết nối tới server*/
    if (connect(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        handle_error("connect()");

    printf("Connected to server\n");
    srand(time(NULL)); // Initialize random seed
    int count = 0;

    while (1)
    {
        count++;
        if (count == 11)
            break;

        int temperature = rand() % 14 + 9; // Generate a random number between 9 and 22
        time_t timestamp = time(NULL);

        snprintf(buffer, BUFFER_SIZE, "%s/%d/%ld", DEVICE_ID, temperature, timestamp);

        // Send message to server
        if (send(server_fd, buffer, strlen(buffer), 0) < 0)
        {
            perror("send failed");
            exit(EXIT_FAILURE);
        }

        printf("Sent message: %s\n", buffer);

        // Clear the buffer
        memset(buffer, 0, BUFFER_SIZE);
        sleep(2);
    }

    close(server_fd);

    return 0;
}