#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

MODULE_AUTHOR("tacolin");
MODULE_DESCRIPTION("WAIT QUEUE KERNEL TEST MODULE");
MODULE_LICENSE("GPL");

extern int register_device(int major, int minor);
extern int unregister_device(int major, int minor);

static int __init kernel_module_init(void)
{
	printk("[taco] %s\n", __func__);
	if ( 0 > register_device(125, 0) )
	{
		printk("[taco-error] register device failed\n");
		return -1;
	}
    return 0;
}

static void __exit kernel_module_exit(void)
{
	printk("[taco] %s\n", __func__);
	if ( 0 > unregister_device(125, 0) )
	{
		printk("[taco-error] unregister device failed\n");
		return;
	}
	return;
}

module_init( kernel_module_init );
module_exit( kernel_module_exit );
