#include <linux/module.h>   /* Defines functions such as module_init/module_exit */
#include <linux/gpio.h>     /* Defines functions such as gpio_request/gpio_free */     

#define GPIO0_30            30   /* Define GPIO pin 30 */
#define LOW                 0    /* Define LOW state for GPIO pin */
#define HIGH                1    /* Define HIGH state for GPIO pin */

#define DRIVER_AUTHOR "TungNHS"   /* Define the author's name */
#define DRIVER_DESC   "gpio subsystem"  /* Define the driver description */

/* Constructor: Initializes the GPIO pin */
static int __init gpio_init(void)
{
    gpio_request(GPIO0_30, "gpio0_30");         /* Request control of GPIO pin 30 */
    gpio_direction_output(GPIO0_30, LOW);       /* Set GPIO pin 30 as an output, initially set to LOW */
    gpio_set_value(GPIO0_30, HIGH);             /* Set GPIO pin 30 to HIGH (turn it on) */

    pr_info("Hello! gpio status: %d!\n", gpio_get_value(GPIO0_30)); /* Print the current status of GPIO pin 30 */
    return 0;
}

/* Destructor: Cleans up the GPIO pin */
static void __exit gpio_exit(void)
{
    gpio_set_value(GPIO0_30, LOW);              /* Set GPIO pin 30 to LOW (turn it off) */
    gpio_free(GPIO0_30);                        /* Release the control of GPIO pin 30 */
    pr_info("Good bye my friend!!!\n");         /* Print exit message */
}

/* Register the initialization and cleanup functions */
module_init(gpio_init);
module_exit(gpio_exit);

MODULE_LICENSE("GPL");                           /* Define the module's license (GPL) */
MODULE_AUTHOR(DRIVER_AUTHOR);                    /* Define the author's name */
MODULE_DESCRIPTION(DRIVER_DESC);                 /* Define the module description */
MODULE_VERSION("1.0");                           /* Define the module version */
