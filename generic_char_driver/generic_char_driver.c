/**
 * @author	Hasan Onuralp AGCA
 * @date	19.04.2024
 * 
 * @brief   Simple Char Device Driver
 *			Steps
 *			major-minor number subscription to the kernel (register_chrdev_region) -> filling "struct file_operations" 
 *			which is encapsulated by "struct cdev" to implement file operations (open, read, write, close, ...) 
 *			when user space app accessed to the device node file these determined functions will be called. -> 
 *			cdev structure is accessed by the kernel with following APIs: cdev_init(), cdev_add(), cdev_del()
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>


#define DEV_MAJOR	25
#define DEV_MINOR	0

MODULE_LICENSE("GPL");
MODULE_AUTHOR("H.Onuralp Agca");
MODULE_DESCRIPTION("General Character Device Driver");

static int generic_open(struct inode *inodep, struct file *filp);
static int generic_release(struct inode *inodep, struct file *filp);
static ssize_t generic_read(struct file *filp, char *buf, size_t size, loff_t *off);
static ssize_t generic_write(struct file *filp, const char *buf, size_t size, loff_t *off);

static struct cdev g_cdev;

static struct file_operations g_fops = {
	.owner = THIS_MODULE, 
	.open = generic_open, 
	.release = generic_release,
	.read = generic_read,
	.write = generic_write
};

static char g_buf[] = "01234567890ABCDEFGH";

static int __init generic_init(void)
{
	int result;

	printk(KERN_INFO "generic char driver module initialization...\n");

	if((result = register_chrdev_region(MKDEV(DEV_MAJOR, DEV_MINOR), 1, "generic-char-driver")) < 0){
		printk(KERN_ERR "cannot register device!..\n");
		return result;
	}

	cdev_init(&g_cdev, &g_fops);

	if((result = cdev_add(&g_cdev, MKDEV(DEV_MAJOR, DEV_MINOR), 1)) < 0){
		unregister_chrdev_region(MKDEV(DEV_MAJOR, DEV_MINOR), 1);
		printk(KERN_ERR "cannot add device!..\n");
		return result;
	}

	return 0;
}

static void __exit generic_exit(void)
{
	cdev_del(&g_cdev);

	unregister_chrdev_region(MKDEV(DEV_MAJOR, DEV_MINOR), 1);

	printk(KERN_INFO "generic-char-driver module exit...\n");
}

static int generic_open(struct inode *inodep, struct file *filp)
{
	printk(KERN_INFO "generic-char-driver opened...\n");

	return 0;
}

static int generic_release(struct inode *inodep, struct file *filp)
{
	printk(KERN_INFO "generic-char-driver-closed...\n");

	return 0;
}

static ssize_t generic_read(struct file *filp, char *buf, size_t size, loff_t *off)
{
	size_t esize;
	size_t slen;

	slen = strlen(g_buf);
	esize = *off + size > slen ? slen - *off : size;

	if (copy_to_user(buf, g_buf + *off, esize) != 0)
		return -EFAULT;

	*off += esize;

	return esize;
}

static ssize_t generic_write(struct file *filp, const char *buf, size_t size, loff_t *off)
{
	printk(KERN_INFO "generic_write function called...\n");

	return size;
}

module_init(generic_init);
module_exit(generic_exit);
