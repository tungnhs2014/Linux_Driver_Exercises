# Character device driver

EXTRA_CFLAGS += -Wall
obj-m += gpio.o

# Paths
BBB_KERNEL := /home/tungnhs/Working_Linux/BBB/bb-kernel/KERNEL/
TOOLCHAIN := /home/tungnhs/Working_Linux/BBB/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
CC := $(TOOLCHAIN)gcc

all: module userapp

# Biên dịch module kernel
module:
	make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) -C $(BBB_KERNEL) M=$(shell pwd) modules

# Biên dịch chương trình user space
userapp:
	$(CC) -o app app.c gpio.c

clean:
	make -C $(BBB_KERNEL) M=$(shell pwd) clean
	rm -rf app

.PHONY: all clean
