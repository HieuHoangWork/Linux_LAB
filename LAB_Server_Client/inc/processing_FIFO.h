#ifndef __PROCESSING_FIFO__
#define __PROCESSING_FIFO__

#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define FIFO_PATH "../tmp/myfifo"
#define LOG_PATH "../tmp/log.txt"

void created_FIFO(const char *fifo_path);
void write_to_FIFO(const char *fifo_path, const char *log_event);
void write_to_LOG_file(const char *fifo_path, const char *log_path);
void handle_error(const char *msg);

#endif