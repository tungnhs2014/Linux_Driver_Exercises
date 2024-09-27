#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdio_ext.h>
#include<sys/ioctl.h>

#define LED_ON  _IOW('a','1',int32_t*)
#define LED_OFF _IOW('a','0',int32_t*)

#define CDEV_PATH "/dev/m_device"

int fd, option;
int number, foo;
char write_buf[1024];
char read_buf[1024];

void printMenu()
{

    printf("****Please Enter the Option******\n");
    printf("        1.  LED ON                \n");
    printf("        2.  LED OFF               \n");
    printf("        0.  Exit                  \n");

    printf(">>> ");
}

int main()
{   
    printf("*********************************\n");
    printf("*******Linux From Scratch*******\n\n");

    fd = open(CDEV_PATH, O_RDWR);
    if (fd < 0) {
        printf("Cannot open device file: %s...\n", CDEV_PATH);
        return -1;
    }

    while(1) {
        printMenu();

        scanf("%d",&option); 
        switch (option) {
            case 1:
                printf("Turn Led ON\n");
                ioctl(fd, LED_ON, (int32_t*)&number);
                printf("Done!\n\n\n");
                break;

            case 2:
                printf("Turn Led OFF\n");
                ioctl(fd, LED_OFF, (int32_t*)&number);
                printf("Done!\n\n\n");
                break;
            case 0:
                close(fd);
                exit(1);               
                break;
            default:
                printf("Enter Valid option = %c\n",option);
                break;
        }
    }
    
    close(fd);

    return 0;
}