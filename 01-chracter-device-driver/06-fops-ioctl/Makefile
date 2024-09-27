# Character device driver

EXTRA_CFLAG = -Wall
obj-m= main.o

KDIR:= /lib/modules/$(shell uname -r)/build
CC := gcc

all:
	make -C $(KDIR) M=$(shell pwd) modules
	$(CC) -o app test_app.c

clean:
	make -C $(KDIR) M=$(shell pwd) clean
	rm -rf app

.PHONY:= all clean
