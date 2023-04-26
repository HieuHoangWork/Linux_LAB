#include "socket_server.h"
#include "processing_linked_list.h"
#include "processing_FIFO.h"

extern int log_sequence_number;
extern volatile sig_atomic_t running;
extern pthread_mutex_t data_mutex;
extern sem_t ser_ready_for_data_mgr;
extern Node *head;

void socket_server_processing(void *args)
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
            // printf("New client connected, IP: %s, Port: %d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

            // Log event: new connection
            char log_event[BUFFER_SIZE];
            sprintf(log_event, "%d %ld A sensor node with IP: %s and Port: %d has opened a new connection.\n", log_sequence_number++, time(NULL), inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
            write_to_FIFO(FIFO_PATH, log_event);

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
                        // printf("Device disconnected, IP: %s, Port: %d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

                        // Log event: closed connection
                        char log_event[BUFFER_SIZE];
                        sprintf(log_event, "%d %ld The sensor node with IP: %s and Port: %d has closed the connection.\n", log_sequence_number++, time(NULL), inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                        write_to_FIFO(FIFO_PATH, log_event);

                        close(sd);
                        client_socket[i] = 0;
                    }
                    else
                    {
                        buffer[valread] = '\0';
                        // printf("Received from client: %s\n", buffer);

                        // Phân tích dữ liệu
                        char sensor_id[32];
                        float temperature;
                        time_t timestamp;

                        sscanf(buffer, "%31[^/]/%f/%ld", sensor_id, &temperature, &timestamp);

                        // Tạo một node mới và thêm vào danh sách liên kết
                        pthread_mutex_lock(&data_mutex);
                        save_data_to_linked_list(&head, atoi(sensor_id), temperature, timestamp);
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