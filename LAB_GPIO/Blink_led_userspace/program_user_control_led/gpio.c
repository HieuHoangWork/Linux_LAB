#include "gpio.h"

/// Set GPIO direction
int GPIO_Direction(const char *direction)
{
        if (strcmp(direction, IN) == 0)
        {
                char path[DIRECTION_MAX];
                snprintf(path, DIRECTION_MAX, "/sys/bbb_gpios/direction");
                int32_t fd;
                fd = open(path, O_WRONLY);
                if (-1 == fd)
                {
                        fprintf(stderr, "Failed to open gpio direction for writing!\n");
                        return (-1);
                }

                if (-1 == write(fd, IN, 2))
                {
                        fprintf(stderr, "Failed to set direction!\n");
                        return (-1);
                }
                close(fd);
        }
        else if (strcmp(direction, OUT) == 0)
        {
                char path[DIRECTION_MAX];
                snprintf(path, DIRECTION_MAX, "/sys/bbb_gpios/direction");
                int32_t fd;
                fd = open(path, O_WRONLY);
                if (-1 == fd)
                {
                        fprintf(stderr, "Failed to open gpio direction for writing!\n");
                        return (-1);
                }

                if (-1 == write(fd, OUT, 3))
                {
                        fprintf(stderr, "Failed to set direction!\n");
                        return (-1);
                }
                close(fd);
        }
        return 0;
}

/// Set GPIO value
int GPIO_Value(int8_t value)
{
        if (value == 0)
        {
                char path[DIRECTION_MAX];
                snprintf(path, DIRECTION_MAX, "/sys/bbb_gpios/value");
                int32_t fd;
                fd = open(path, O_WRONLY);
                if (-1 == fd)
                {
                        fprintf(stderr, "Failed to open gpio direction for writing!\n");
                        return (-1);
                }

                char value_str[2];
                snprintf(value_str, sizeof(value_str), "%d", 0);

                if (-1 == write(fd, value_str, strlen(value_str)))
                {
                        fprintf(stderr, "Failed to write value!\n");
                        return (-1);
                }
                close(fd);
        }
        else if (value == 1)
        {
                char path[DIRECTION_MAX];
                snprintf(path, DIRECTION_MAX, "/sys/bbb_gpios/value");
                int32_t fd;
                fd = open(path, O_WRONLY);
                if (-1 == fd)
                {
                        fprintf(stderr, "Failed to open gpio direction for writing!\n");
                        return (-1);
                }

                char value_str[2];
                snprintf(value_str, sizeof(value_str), "%d", 1);

                if (-1 == write(fd, value_str, strlen(value_str)))
                {
                        fprintf(stderr, "Failed to write value!\n");
                        return (-1);
                }
                close(fd);
        }
        return 0;
}