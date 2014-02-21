#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>

#include <linux/cdev.h>
#include <asm/uaccess.h>

#include <linux/device.h>

static int _device_open(struct inode*, struct file*);
static int _device_release(struct inode*, struct file*);
static ssize_t _device_write(struct file*, const char*, size_t, loff_t*);
static ssize_t _device_read(struct file*, char*, size_t, loff_t*);

static struct file_operations _ops = {
	open:		_device_open,
	release:	_device_release,
	write:		_device_write,
	read:		_device_read,
};

static struct class* 	_p_class = NULL;
static struct cdev 		_cdevice;

static unsigned char 	_kbuf[128];

static int _device_open(struct inode* p_node, struct file* p_file)
{
	printk("[KERNEL-PART] %s\n", __func__);
	return 0;
}

static int _device_release(struct inode* p_node, struct file* p_file)
{
	printk("[KERNEL-PART] %s\n", __func__);
	return 0;
}

static ssize_t _device_write(struct file* p_file, const char* p_data, size_t count, loff_t* p_position)
{
	printk("[KERNEL-PART] %s\n", __func__);

	count = min( (unsigned long)128, (unsigned long)count );

	if ( 0 > copy_from_user( _kbuf, p_data, count) )
	{
		printk("[KERNEL-PART-ERROR] copy from user failed\n");
		return -EFAULT;
	}

	printk("[KERNEL-PART] %s\n", _kbuf);	

	return count;
}

static ssize_t _device_read(struct file* p_file, char *p_buf, size_t len, loff_t* p_position)
{
	size_t read_len = min( (size_t)128, len );

	printk("[KERNEL-PART] %s\n", __func__);

	if ( 0 > copy_to_user( p_buf, _kbuf, read_len ) )
	{
		printk("[KERNEL-PART-ERROR] copy to user failed\n");
		return -EFAULT;
	}
	
	return read_len;
}


int register_device(int major, int minor)
{
	dev_t device_number;
	int   ret;

	device_number = MKDEV(major, minor);
	if ( !MAJOR(device_number) )
	{
		ret = alloc_chrdev_region( &device_number, MINOR(device_number), 1, "tacobuf");
		printk("[KERNEL-PART] alloc_chrdev_region\n");
	}
	else
	{
		ret = register_chrdev_region(device_number, 1, "tacobuf");
		printk("[KERNEL-PART] register_chrdev_region\n");
	}

	if ( ret < 0 )
	{
		printk("[KERNEL-PART-ERROR] register device failed\n");
		return ret;
	}

	memset(&_kbuf, 0, 128);
	sprintf(_kbuf, "nothing");
	memset(&_cdevice, 0, sizeof(struct cdev));

	cdev_init( &_cdevice, &_ops );

	_cdevice.owner = THIS_MODULE;
	_cdevice.ops   = &_ops;

	ret = cdev_add( &_cdevice, device_number, 1 );
	if ( ret != 0 )
	{
		unregister_chrdev_region( device_number, 1 );
		printk("[KERNEL-PART-ERROR] add device failed\n");
		return ret;
	}
	
	_p_class = class_create(THIS_MODULE, "taco_class");
	if ( _p_class == NULL )
	{
		printk("[KERNEL-PART-ERROR] class create failed\n");
		unregister_chrdev_region( device_number, 1 );
		return -1;
	}

	if ( NULL == device_create(_p_class, NULL, device_number, NULL, "tacobuf") )
	{
		printk("[KERNEL-PART-ERROR] device create failed\n");
		unregister_chrdev_region( device_number, 1 );
		return -1;
	}

	return 0;
}

int unregister_device(int major, int minor)
{
	dev_t device_number;

	device_number = MKDEV(major, minor);

	cdev_del( &_cdevice );

	unregister_chrdev_region(device_number, 1);

	device_destroy(_p_class, device_number);
	
	class_destroy(_p_class);

	return 0;
}

