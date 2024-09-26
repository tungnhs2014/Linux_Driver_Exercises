#include <linux/module.h>   // Header for all kernel modules
#include <linux/io.h>       // Header for memory-mapped I/O functions
#include "gpio.h"           // Custom header for GPIO base addresses and offsets

// Macros defining author, description, and version of the driver
#define DRIVER_AUTHOR   "TungNHS"
#define DRIVER_DESC     "Turn on and off LED"
#define DRIVER_VERS     "1.0"

uint32_t __iomem *gpio0_base_addr;   // Pointer to hold the mapped base address of GPIO0

/* Constructor: Initializes the driver */
static int __init GPIO_init(void)
{
    // Map the physical memory address of GPIO0 into the virtual address space
    gpio0_base_addr = ioremap(GPIO0_ADDR_BASE, GPIO0_ADDR_SIZE);

    // Check if the mapping was successful; return error if not
    if (!gpio0_base_addr)
    {
        return -ENOMEM;  // Return error code for memory allocation failure
    }

    // Set GPIO pin 30 as output by clearing the corresponding bit in the Output Enable register
    *(gpio0_base_addr + GPIO_OE_OFFSET / 4) &= ~GPIO0_30;

    // Set the pin high (turn the LED on) by setting the corresponding bit in the Set Data Out register
    *(gpio0_base_addr + GPIO_SETDATAOUT_OFFSET / 4) |= GPIO0_30;

    pr_info("Initialized successfully!\n");  // Log successful initialization
    
    return 0;  // Return 0 for successful initialization
}

/* Destructor: Cleans up the driver before exit */
static void __exit GPIO_exit(void)
{
    // Clear the GPIO pin 30 (turn the LED off) by setting the corresponding bit in the Clear Data Out register
    *(gpio0_base_addr + GPIO_CLEARDATAOUT_OFFSET / 4) |= GPIO0_30;

    // Unmap the memory that was mapped using ioremap
    iounmap(gpio0_base_addr);

    pr_info("Uninitialized successfully!\n");  // Log successful uninitialization
}

/* Register the init and exit functions */
module_init(GPIO_init);
module_exit(GPIO_exit);

// Provide information about the module
MODULE_LICENSE("GPL");                  // Declare the module's license as GPL
MODULE_AUTHOR(DRIVER_AUTHOR);           // Declare the module author
MODULE_DESCRIPTION(DRIVER_DESC);        // Short description of the module
MODULE_VERSION(DRIVER_VERS);            // Version of the module
