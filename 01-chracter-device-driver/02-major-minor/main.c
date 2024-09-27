#include <linux/module.h>   /* Provides module_init(), module_exit() macros */
#include <linux/fs.h>       /* Provides alloc_chrdev_region(), register_chrdev_region() */

#define DRIVER_AUTHOR 	"TungNHS"  // Author information
#define DRIVER_DESC	    "Hello world kernel module" // Descriptin of the module
#define DRIVER_VERS     "1.0" // Version of the module

// Structure to represent a device, holding the device number
struct m_foo_dev
{
	dev_t dev_num; // Variable to store device number (major and minor)
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

    /* 1.1 Static allocating device number (cat /proc/devices) */
    // dev_t dev = MKDEV(173, 0);
    // register_chrdev_region(&mdev.dev_num, 1, "m-cdev")
	
	// If successful, print the allocated major and minor numbers
	pr_info("Major = %d, Minor: %d\n", MAJOR(mdev.dev_num), MINOR(mdev.dev_num));

    // Print a message to indicate the module has been loaded
	pr_info("Hello world kernel module!!\n");
	return 0;
}


/* Destructor function (called when the module is unloaded) */
static void __exit chdev_exit(void)
{
	// Unregister the allocated device number (free the region)
    unregister_chrdev_region(mdev.dev_num, 1);

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
 
