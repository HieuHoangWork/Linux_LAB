# Paths
BBB_KERNEL := /home/hieuhoangwork/work/BBB/bb-kernel/KERNEL
TOOLCHAIN_USERSPACE := /home/hieuhoangwork/work/BBB/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
TOOLCHAIN_ARM := /home/hieuhoangwork/work/BBB/bb-kernel/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-

# Toolchain
CC := $(TOOLCHAIN_USERSPACE)gcc

all :
	make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN_ARM) -C $(BBB_KERNEL) M=$(shell pwd) modules

clean :
	make -C $(BBB_KERNEL) M=$(shell pwd) clean