# Character device driver

EXTRA_CFLAG = -Wall
obj-m= led.o

BBB_KERNEL:= /home/tungnhs/Working_Linux/BBB/bb-kernel/KERNEL/
TOOLCHAIN:= /home/tungnhs/Working_Linux/BBB/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
CC_CHAIN:= /home/tungnhs/Working_Linux/BBB/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-

all:
	make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) -C $(BBB_KERNEL) M=$(shell pwd) modules
	$(CC_CHAIN)gcc -o app app.c

clean:
	make -C $(BBB_KERNEL) M=$(shell pwd) clean
	rm -rf app

.PHONY:= all clean
