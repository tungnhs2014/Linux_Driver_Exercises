#include <linux/module.h>   /* Provides module_init(), module_exit() macros */
#include <linux/fs.h>       /* Provides alloc_chrdev_region(), register_chrdev_region() */
#include <linux/device.h>   /* Provides device_create(), class_create() */
#include <linux/cdev.h>     /* Provides cdev_init(), cdev_add() */

#define DRIVER_AUTHOR 	"TungNHS"  // Author information
#define DRIVER_DESC	    "Hello world kernel module" // Description of the module
#define DRIVER_VERS     "1.0" // Version of the module

// Structure to represent a device, holding the device number, class, and cdev structure
struct m_foo_dev
{
	dev_t dev_num;           // Variable to store device number (major and minor)
    struct class *m_class;   // Pointer to device class
    struct cdev m_cdev;      // Character device structure

} mdev;

/*  Function Prototypes */
static int      __init chdev_init(void);            // Module init function
static void     __exit chdev_exit(void);            // Module exit function
static int      m_open(struct inode *inode, struct file *file);         // open() system call handler
static int      m_release(struct inode *inode, struct file *file);      // release() system call handler
static ssize_t  m_read(struct file *filp, char __user *user_buf, size_t size,loff_t * offset);  // read() system call handler
static ssize_t  m_write(struct file *filp, const char *user_buf, size_t size, loff_t * offset); // write() system call handler

// File operations structure with pointers to device operations (open, read, write, release)
static struct file_operations fops =
{
    .owner      = THIS_MODULE,  // Module that owns this struct
    .read       = m_read,       // read() function pointer
    .write      = m_write,      // write() function pointer
    .open       = m_open,       // open() function pointer
    .release    = m_release,    // release() function pointer
};

/* This function is called when the device file is opened */
static int m_open(struct inode *inode, struct file *file)
{
    pr_info("System call open() called...!!!\n");
    return 0;  // Successful opening
}

/* This function is called when the device file is closed */
static int m_release(struct inode *inode, struct file *file)
{
    pr_info("System call close() called...!!!\n");
    return 0;  // Successful closing
}

/* This function is called when the device file is read */
static ssize_t m_read(struct file *filp, char __user *user_buf, size_t size, loff_t *offset)
{
    pr_info("System call read() called...!!!\n");
    return 0;  // Successfully handled read operation (returns no data)
}

/* This function is called when the device file is written */
static ssize_t m_write(struct file *filp, const char __user *user_buf, size_t size, loff_t *offset)
{
    pr_info("System call write() called...!!!\n");
    return size;  // Return the number of bytes written (fake write)
}

/* Constructor function (called when the module is loaded) */
static int __init chdev_init(void)
{
	// 1. Dynamically allocate a device number (visible via 'cat /proc/devices')
	// alloc_chrdev_region takes a pointer to dev_num, starting minor number (0),
	// number of devices (1 in this case), and a device name ("m_cdev").
	if(alloc_chrdev_region(&mdev.dev_num, 0, 1, "m_cdev") < 0)
	{
		// If allocation fails, log an error message and return -1 (error)
		pr_err("Failed to allocate chrdev region\n");
		return -1;
	}

	// Print the allocated major and minor numbers
	pr_info("Major = %d, Minor = %d\n", MAJOR(mdev.dev_num), MINOR(mdev.dev_num));

    // 2. Create the struct class for the device
    // class_create allocates a class structure that can be used to manage devices in sysfs.
    if((mdev.m_class = class_create(THIS_MODULE, "m_class")) == NULL)
    {
        pr_err("Cannot create struct class for my device\n");
        goto rm_device_number; // If class creation fails, cleanup and exit
    }

    // 3. Create the device node in /dev (visible as /dev/m_device)
    if((device_create(mdev.m_class, NULL, mdev.dev_num, NULL, "m_device")) == NULL)
    {
        pr_err("Cannot create device\n");
        goto rm_class;  // If device creation fails, cleanup and exit
    }

    // 4. Initialize the cdev structure with file operations
    cdev_init(&mdev.m_cdev, &fops);

    // 4.1 Add the cdev to the system
    if((cdev_add(&mdev.m_cdev, mdev.dev_num, 1)) < 0 )
    {
        pr_err("Cannot add cdev to system\n");
        goto rm_device;  // If cdev addition fails, cleanup and exit
    }

    // Successfully initialized module
	pr_info("Hello world kernel module!!\n");
	return 0;

rm_device:
    device_destroy(mdev.m_class, mdev.dev_num);  // Destroy device on error

rm_class:  // Error handling: destroy the created class if device creation fails
    class_destroy(mdev.m_class);

rm_device_number:  // Error handling: unregister device number if class creation fails
    unregister_chrdev_region(mdev.dev_num, 1);
    return -1;
}

/* Destructor function (called when the module is unloaded) */
static void __exit chdev_exit(void)
{
    
    cdev_del(&mdev.m_cdev);                                 // Remove the character device from the system
    device_destroy(mdev.m_class, mdev.dev_num);             // Destroy the device
    class_destroy(mdev.m_class);                            // Destroy the class
    unregister_chrdev_region(mdev.dev_num, 1);              // Unregister the device number
    pr_info("Goodbye kernel module!!\n");
}

// Register chdev_init as the initialization function and chdev_exit as the exit function
module_init(chdev_init);
module_exit(chdev_exit);

// Provide metadata about the module
MODULE_LICENSE("GPL"); // License for the module (GPL)
MODULE_AUTHOR(DRIVER_AUTHOR);    // Information about the author
MODULE_DESCRIPTION(DRIVER_DESC);  // Description of the module
MODULE_VERSION(DRIVER_VERS);  // Version of the module 
