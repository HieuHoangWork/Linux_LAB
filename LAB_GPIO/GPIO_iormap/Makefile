# Paths
BBB_KERNEL := /home/hieuhoangwork/work/BBB/bb-kernel/KERNEL/
TOOLCHAIN := /home/hieuhoangwork/work/BBB/bb-kernel/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin

# Toolchain
PREFIX := arm-linux-gnueabi-
CC := gcc

all:
	make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN)/$(PREFIX) -C $(BBB_KERNEL) M=$(shell pwd) modules
	
clean:
	make -C $(BBB_KERNEL) M=$(shell pwd) clean