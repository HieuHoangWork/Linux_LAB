#ifndef GPIO_H
#define GPIO_H

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#define IN "in"   ///< GPIO direction as input
#define OUT "out" ///< GPIO direction as output

#define LOW 0  ///< GPIO value Low
#define HIGH 1 ///< GPIO value High

#define DIRECTION_MAX 100

int GPIO_Direction(const char *direction);
int GPIO_Value(int8_t value);

#endif /// End of GPIO_H
