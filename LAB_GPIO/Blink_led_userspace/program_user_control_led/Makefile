.PHONY: app clean

CC := /home/hieuhoangwork/work/BBB/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc
CFLAGS := -I.

INC_FILES := gpio.h

%.o: %.c $(INC_FILES)
	$(CC) -I. -c -o $@ $<

app: main.o gpio.o
	$(CC) -o $@ main.o gpio.o $(CFLAGS)
clean:
	rm -rf app *.o