/****************************************
 * @brief Userspace application to test gpio - sysfs
 * @author TungNHS
 ****************************************/

#include <stdio.h>
#include "gpio.h"

#define LED (31)

int main(void)
{
    printf("******* Linux From Scratch *******\n");

    GPIOExport(LED);
    GPIODirection(LED, OUT);
    GPIOWrite(LED, HIGH);

    return 0;
}
