
#GPIO Driver Legacy
BBB_KERNEL:= /home/tungnhs/Working_Linux/BBB/bb-kernel/KERNEL/
TOOLCHAIN:= /home/tungnhs/Working_Linux/BBB/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-

EXTRA_CFLAG:=-Wall
obj-m:= gpio_sysfs.o

all:
	make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) -C $(BBB_KERNEL) M=$(shell pwd) modules

clean:
	make -C $(BBB_KERNEL) M=$(shell pwd) clean