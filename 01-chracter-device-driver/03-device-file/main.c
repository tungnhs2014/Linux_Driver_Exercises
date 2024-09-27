#include <linux/module.h>   /* Provides module_init(), module_exit() macros */
#include <linux/fs.h>       /* Provides alloc_chrdev_region(), register_chrdev_region() */
#include <linux/device.h>   /* Provides device_create(), class_create() */

#define DRIVER_AUTHOR 	"TungNHS"  // Author information
#define DRIVER_DESC	    "Hello world kernel module" // Description of the module
#define DRIVER_VERS     "1.0" // Version of the module

// Structure to represent a device, holding the device number and class
struct m_foo_dev
{
	dev_t dev_num;           // Variable to store device number (major and minor)
    struct class *m_class;   // Pointer to device class
} mdev;


/* Constructor function (called when the module is loaded) */
static int __init chdev_init(void)
{
	// 1. Dynamically allocate a device number (visible via 'cat /proc/devices')
	// alloc_chrdev_region takes a pointer to dev_num, starting minor number (0),
	// number of devices (1 in this case), and a device name ("m_cdev").
	if(alloc_chrdev_region(&mdev.dev_num, 0, 1, "m_cdev") < 0)
	{
		// If allocation fails, log an error message and return -1 (error)
		pr_err("Failed to alloc chrdev region\n");
		return -1;
	}

    // 1.1 Static device number allocation (not used in this example)
    // dev_t dev = MKDEV(173, 0);
    // register_chrdev_region(&mdev.dev_num, 1, "m-cdev");

	// If successful, print the allocated major and minor numbers
	pr_info("Major = %d, Minor: %d\n", MAJOR(mdev.dev_num), MINOR(mdev.dev_num));

    // 2.0 Creating struct class for the device
    // class_create allocates a class structure that can be used to create devices.
    if((mdev.m_class = class_create(THIS_MODULE, "m_class")) == NULL)
    {
        pr_err("Cannot create the struct class for my device\n");
        goto rm_device_number; // If class creation fails, cleanup and exit
    }

    // 3.0 Creating the device
    // device_create registers a device, visible via /dev/m_device
    if((device_create(mdev.m_class, NULL, mdev.dev_num, NULL, "m_device")) == NULL)
    {
        pr_err("Cannot create my device\n");
        goto rm_class;  // If device creation fails, cleanup and exit
    }

    // Print a message to indicate the module has been loaded
	pr_info("Hello world kernel module!!\n");
	return 0;

rm_class:  // Error handling: destroy the created class if device creation fails
    class_destroy(mdev.m_class);

rm_device_number:  // Error handling: unregister device number if class creation fails
    unregister_chrdev_region(mdev.dev_num, 1);
    return -1;
}

/* Destructor function (called when the module is unloaded) */
static void __exit chdev_exit(void)
{
    // Clean up the device and class that were created during initialization
    device_destroy(mdev.m_class, mdev.dev_num);             /* 3.0 */
    class_destroy(mdev.m_class);                            /* 2.0 */
    unregister_chrdev_region(mdev.dev_num, 1);              /* 1.0 */
    
    // Print a message indicating the module has been unloaded
	pr_info("Good bye!!\n");
}

// Register chdev_init as the initialization function and chdev_exit as the exit function
module_init(chdev_init);
module_exit(chdev_exit);

// Provide metadata about the module
MODULE_LICENSE("GPL"); // License for the module (GPL)
MODULE_AUTHOR(DRIVER_AUTHOR);    // Information about the author
MODULE_DESCRIPTION(DRIVER_DESC);  // Description of the module
MODULE_VERSION(DRIVER_VERS);  // Version of the module 
