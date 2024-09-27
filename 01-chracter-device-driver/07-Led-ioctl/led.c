#include <linux/module.h>   /* Provides module_init(), module_exit() macros */
#include <linux/fs.h>       /* Provides alloc_chrdev_region(), register_chrdev_region() */
#include <linux/device.h>   /* Provides device_create(), class_create() */
#include <linux/cdev.h>     /* Provides cdev_init(), cdev_add() */
#include <linux/uaccess.h>  /* Provides copy_to_user(), copy_from_user() */
#include <linux/slab.h>     /* Provides kmalloc() and kfree() */

#include <linux/module.h>   // Header for all kernel modules
#include <linux/io.h>       // Header for memory-mapped I/O functions
#include "led.h"           // Custom header for GPIO base addresses and offsets

#define DRIVER_AUTHOR 	"TungNHS"  // Author information
#define DRIVER_DESC	    "LED on and off kenerl module" // Description of the module
#define DRIVER_VERS     "1.0" // Version of the module

#define NPAGES  1  // Number of pages to allocate in kernel memory

#define LED_ON  _IOW('a', '1', int32_t *)
#define LED_OFF _IOW('a', '0', int32_t *)

uint32_t __iomem *gpio0_base_addr;   // Pointer to hold the mapped base address of GPIO0

// Structure to represent a device, holding the device number, class, cdev, and memory
struct m_foo_dev
{
    int32_t size;              // Current size of data written to the device
    char *kmalloc_ptr;         // Pointer to the allocated kernel memory
	dev_t dev_num;             // Variable to store device number (major and minor)
    struct class *m_class;     // Pointer to device class
    struct cdev m_cdev;        // Character device structure

} mdev;

/*  Function Prototypes */
static int      __init led_init(void);
static void     __exit led_exit(void);
static long     m_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .unlocked_ioctl = m_ioctl,
};

static long m_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch(cmd) {
        case LED_ON:
            pr_info("LED ON!\n");
            // Set the pin high (turn the LED on) by setting the corresponding bit in the Set Data Out register
            *(gpio0_base_addr + GPIO_SETDATAOUT_OFFSET / 4) |= GPIO0_30;
            break;

        case LED_OFF:
            pr_info("LED OFF!\n");
            // Clear the GPIO pin 30 (turn the LED off) by setting the corresponding bit in the Clear Data Out register
            *(gpio0_base_addr + GPIO_CLEARDATAOUT_OFFSET / 4) |= GPIO0_30;
            break;

        default:
                pr_info("Default\n");
                break;
    }

    return 0;
}

/* Constructor function (called when the module is loaded) */
static int __init led_init(void)
{
	// 1. Dynamically allocate a device number
	if(alloc_chrdev_region(&mdev.dev_num, 0, 1, "m_cdev") < 0)
	{
		pr_err("Failed to allocate chrdev region\n");
		return -1;
	}

	// Print the allocated major and minor numbers
	pr_info("Major = %d, Minor = %d\n", MAJOR(mdev.dev_num), MINOR(mdev.dev_num));

    // 2. Create the struct class for the device
    if((mdev.m_class = class_create(THIS_MODULE, "m_class")) == NULL)
    {
        pr_err("Cannot create struct class for my device\n");
        goto rm_device_number;
    }

    // 3. Create the device node in /dev (visible as /dev/m_device)
    if((device_create(mdev.m_class, NULL, mdev.dev_num, NULL, "m_device")) == NULL)
    {
        pr_err("Cannot create device\n");
        goto rm_class;
    }

    // 4. Initialize the cdev structure with file operations
    cdev_init(&mdev.m_cdev, &fops);

    // 4.1 Add the cdev to the system
    if((cdev_add(&mdev.m_cdev, mdev.dev_num, 1)) < 0 )
    {
        pr_err("Cannot add cdev to system\n");
        goto rm_device;
    }

    /* 5. Allocate memory in the kernel using kmalloc */
    if((mdev.kmalloc_ptr = kmalloc(NPAGES * PAGE_SIZE, GFP_KERNEL)) == 0)
    {
        pr_err("Cannot allocate memory in kernel\n");
        goto rm_device;
    }

    // Map the physical memory address of GPIO0 into the virtual address space
    gpio0_base_addr = ioremap(GPIO0_ADDR_BASE, GPIO0_ADDR_SIZE);

    // Check if the mapping was successful; return error if not
    if (!gpio0_base_addr)
    {
        return -ENOMEM;  // Return error code for memory allocation failure
    }

    // Set GPIO pin 30 as output by clearing the corresponding bit in the Output Enable register
    *(gpio0_base_addr + GPIO_OE_OFFSET / 4) &= ~GPIO0_30;

    // Successfully initialized module
	pr_info("initialized LED kernel module!!\n");
	return 0;

rm_device:
    device_destroy(mdev.m_class, mdev.dev_num);  // Destroy device on error

rm_class:
    class_destroy(mdev.m_class);  // Destroy the class on error

rm_device_number:
    unregister_chrdev_region(mdev.dev_num, 1);  // Unregister device number on error
    return -1;
}

/* Destructor function (called when the module is unloaded) */
static void __exit led_exit(void)
{
    // Unmap the memory that was mapped using ioremap
    iounmap(gpio0_base_addr);

    kfree(mdev.kmalloc_ptr);  // Free the allocated kernel memory
    cdev_del(&mdev.m_cdev);   // Remove the character device from the system
    device_destroy(mdev.m_class, mdev.dev_num);  // Destroy the device
    class_destroy(mdev.m_class);  // Destroy the class
    unregister_chrdev_region(mdev.dev_num, 1);  // Unregister the device number
    
    pr_info("Goodbye LED kernel module!!\n");
}

// Register led_init as the initialization function and led_exit as the exit function
module_init(led_init);
module_exit(led_exit);

// Provide metadata about the module
MODULE_LICENSE("GPL");  // License for the module (GPL)
MODULE_AUTHOR(DRIVER_AUTHOR);  // Information about the author
MODULE_DESCRIPTION(DRIVER_DESC);  // Description of the module
MODULE_VERSION(DRIVER_VERS);  // Version of the module
