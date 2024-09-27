#include <linux/module.h>   /* Provides module_init(), module_exit() macros */
#include <linux/fs.h>       /* Provides alloc_chrdev_region(), register_chrdev_region() */
#include <linux/device.h>   /* Provides device_create(), class_create() */
#include <linux/cdev.h>     /* Provides cdev_init(), cdev_add() */
#include <linux/uaccess.h>  /* Provides copy_to_user(), copy_from_user() */
#include <linux/slab.h>     /* Provides kmalloc() and kfree() */

#define DRIVER_AUTHOR 	"TungNHS"  // Author information
#define DRIVER_DESC	    "Hello world kernel module" // Description of the module
#define DRIVER_VERS     "1.0" // Version of the module

#define NPAGES  1  // Number of pages to allocate in kernel memory

// Structure to represent a device, holding the device number, class, cdev, and memory
struct m_foo_dev
{
    int32_t size;              // Current size of data written to the device
    char *kmalloc_ptr;         // Pointer to the allocated kernel memory
	dev_t dev_num;             // Variable to store device number (major and minor)
    struct class *m_class;     // Pointer to device class
    struct cdev m_cdev;        // Character device structure

} mdev;

/* Function Prototypes */
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

/* This function is called when we read from the device file */
static ssize_t m_read(struct file *filp, char __user *user_buf, size_t size, loff_t *offset)
{
    size_t to_read;

    pr_info("System call read() called...!!!\n");

    /* Check size does not exceed the data written to the device */
    to_read = (size > mdev.size - *offset) ? (mdev.size - *offset) : size;

	/* Copy from kernel space to user space */
	if (copy_to_user(user_buf, mdev.kmalloc_ptr + *offset, to_read))
		return -EFAULT;

    *offset += to_read;

	return to_read;  // Return the number of bytes read
}

/* This function is called when we write to the device file */
static ssize_t m_write(struct file *filp, const char __user *user_buf, size_t size, loff_t *offset)
{
    size_t to_write;

    pr_info("System call write() called...!!!\n");

    /* Check that we do not write beyond the allocated memory */
    to_write = (size + *offset > NPAGES * PAGE_SIZE) ? (NPAGES * PAGE_SIZE - *offset) : size;

    /* Clear the kernel buffer and then copy from user space to kernel space */
    memset(mdev.kmalloc_ptr, 0, NPAGES * PAGE_SIZE);
    if (copy_from_user(mdev.kmalloc_ptr + *offset, user_buf, to_write) != 0)
        return -EFAULT;

    pr_info("Data from user: %s", mdev.kmalloc_ptr);

    *offset += to_write;
    mdev.size = *offset;  // Update the size to reflect the data written

    return to_write;  // Return the number of bytes written
}

/* Constructor function (called when the module is loaded) */
static int __init chdev_init(void)
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

    // Successfully initialized module
	pr_info("Hello world kernel module!!\n");
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
static void __exit chdev_exit(void)
{
    kfree(mdev.kmalloc_ptr);  // Free the allocated kernel memory
    cdev_del(&mdev.m_cdev);   // Remove the character device from the system
    device_destroy(mdev.m_class, mdev.dev_num);  // Destroy the device
    class_destroy(mdev.m_class);  // Destroy the class
    unregister_chrdev_region(mdev.dev_num, 1);  // Unregister the device number
    
    pr_info("Goodbye kernel module!!\n");
}

// Register chdev_init as the initialization function and chdev_exit as the exit function
module_init(chdev_init);
module_exit(chdev_exit);

// Provide metadata about the module
MODULE_LICENSE("GPL");  // License for the module (GPL)
MODULE_AUTHOR(DRIVER_AUTHOR);  // Information about the author
MODULE_DESCRIPTION(DRIVER_DESC);  // Description of the module
MODULE_VERSION(DRIVER_VERS);  // Version of the module
