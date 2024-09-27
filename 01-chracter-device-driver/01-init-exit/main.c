#include <linux/module.h>	/*Define module_init(), module_exit()*/

#define DRIVER_AUTHOR 	"TungNHS"  // Author information
#define DRIVER_DESC	"Hello world kernel module"  // Description of the module
#define DRVIER_VERS	"1.0" // Version of the module

/* Constructor function (called when the module is loaded) */
static int __init chdev_init(void)
{
	pr_info("Hello world kernel module!!\n");
	return 0;
}


/* Destructor function (called when the module is unloaded) */
static void __exit chdev_exit(void)
{
	pr_info("Good bye!!\n");
}

// Register chdev_init as the initialization function and chdev_exit as the exit function
module_init(chdev_init);
module_exit(chdev_exit);

// Provide metadata about the module
MODULE_LICENSE("GPL"); // License for the module (GPL)
MODULE_AUTHOR(DRIVER_AUTHOR);    // Information about the author
MODULE_DESCRIPTION(DRIVER_DESC);  // Description of the module
MODULE_VERSION(DRVIER_VERS);  // Version of the module
 