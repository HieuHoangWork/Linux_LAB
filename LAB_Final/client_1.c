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
#include <time.h>

#define MAX_BUFFER_SIZE 2048
#define DEVICE_ID "DEVICE_1"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: ./client <hostname> <port number>\n");
        exit(EXIT_FAILURE);
    }

    int client_fd;
    struct sockaddr_in server_address;
    struct hostent *server;

    char buffer[MAX_BUFFER_SIZE];

    // Create client socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR: No such host\n");
        exit(EXIT_FAILURE);
    }

    // Set server address
    memset((char *)&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    memcpy((char *)&server_address.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
    server_address.sin_port = htons(atoi(argv[2]));

    // Connect to server
    if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");
    srand(time(NULL)); // Initialize random seed

    while (1)
    {
        int temperature = rand() % 14 + 9; // Generate a random number between 9 and 22
        time_t timestamp = time(NULL);

        snprintf(buffer, MAX_BUFFER_SIZE, "/%s/%d/%ld", DEVICE_ID, temperature, timestamp);

        // Send message to server
        if (send(client_fd, buffer, strlen(buffer), 0) < 0)
        {
            perror("send failed");
            exit(EXIT_FAILURE);
        }

        printf("Sent message: %s\n", buffer);

        // Clear the buffer
        memset(buffer, 0, MAX_BUFFER_SIZE);

        sleep(2); // You can adjust the time interval between sending data here
    }

    close(client_fd);

    return 0;
}