# Character device driver

EXTRA_CFLAG = -Wall
obj-m= main.o

KDIR:= /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(shell pwd) modules

clean:
	make -C $(KDIR) M=$(shell pwd) clean

.PHONY:= all clean
