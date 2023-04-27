#include "processing_FIFO.h"
#include "processing_linked_list.h"

// Hàm xử lý lỗi
void handle_error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void created_FIFO(const char *fifo_path)
{
    if (mkfifo(fifo_path, 0666) == -1)
    {
        handle_error("mkfifo");
    }
}

void write_to_FIFO(const char *fifo_path, const char *log_event)
{
    int fifo_fd = open(fifo_path, O_RDWR);
    if (fifo_fd == -1)
    {
        handle_error("open");
    }

    ssize_t bytes_written = write(fifo_fd, log_event, strlen(log_event));
    if (bytes_written == -1)
    {
        handle_error("write");
    }

    close(fifo_fd);
}

void write_to_LOG_file(const char *fifo_path, const char *log_path)
{
    int fifo_fd = open(fifo_path, O_RDONLY);
    if (fifo_fd == -1)
    {
        handle_error("open");
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(fifo_fd, buffer, BUFFER_SIZE - 1);
    if (bytes_read == -1)
    {
        handle_error("read");
    }
    buffer[bytes_read] = '\0';

    close(fifo_fd);

    if (bytes_read > 0)
    {
        FILE *log_file = fopen(log_path, "a");
        if (!log_file)
        {
            handle_error("fopen");
        }

        fputs(buffer, log_file);
        fclose(log_file);
    }
}