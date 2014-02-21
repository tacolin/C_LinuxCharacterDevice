#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

MODULE_AUTHOR("KERNEL-PARTlin");
MODULE_DESCRIPTION("CHARACTOR DEVICE KERNEL TEST MODULE 1");
MODULE_LICENSE("GPL");

extern int register_device(int major, int minor);
extern int unregister_device(int major, int minor);

static int __init kernel_module_init(void)
{
	printk("[KERNEL-PART] %s\n", __func__);
	if ( 0 > register_device(125, 0) )
	{
		printk("[KERNEL-PART-ERROR] register device failed\n");
		return -1;
	}
    return 0;
}

static void __exit kernel_module_exit(void)
{
	printk("[KERNEL-PART] %s\n", __func__);
	if ( 0 > unregister_device(125, 0) )
	{
		printk("[KERNEL-PART-ERROR] unregister device failed\n");
		return;
	}
	return;
}

module_init( kernel_module_init );
module_exit( kernel_module_exit );
