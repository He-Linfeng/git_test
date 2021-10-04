#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include"hello.h"

module_param(major,int,S_IRUGO);
module_param(minor,int,S_IRUGO);

static int hello_dev_open(struct inode *inode, struct file *file)
{
    try_module_get(THIS_MODULE);
    printk(KERN_INFO HELLO_DEVICE_NODE_NAME " opened!\n");
    return 0;
}

static int hello_dev_release(struct inode *inode, struct file *file)
{ 
    printk(KERN_INFO HELLO_DEVICE_NODE_NAME " closed!\n");
    module_put(THIS_MODULE);
    return 0;
}

static ssize_t hello_dev_read(struct file *file, char *buf, size_t count, loff_t *f_pos)
{
    printk(KERN_INFO HELLO_DEVICE_NODE_NAME " read method!\n");
    return count;
}

static ssize_t hello_dev_write(struct file *file, const char *buf, size_t count, loff_t *f_pos)
{
    printk(KERN_INFO HELLO_DEVICE_NODE_NAME " write method!\n");
    return count;
}

static long hello_dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    printk(KERN_INFO HELLO_DEVICE_NODE_NAME " ioctl method!\n");
    return 0;
}

struct file_operations hello_cdev_fops = {
    .owner = THIS_MODULE,
    .read = hello_dev_read,
    .write = hello_dev_write,
    .open = hello_dev_open,
    .release = hello_dev_release,
    .unlocked_ioctl = hello_dev_ioctl
};

static int __init hello_dev_init(void)
{
    int ret;
    
    /*获取设备编号*/
    if(major>0)
    {
        /*静态获取设备号*/
        devno = MKDEV(major,minor);
        ret = register_chrdev_region(devno,1,HELLO_DEVICE_NODE_NAME);
    }
    else
    {
        /*动态获取设备号*/
        devno = MKDEV(major,minor);
        ret = alloc_chrdev_region(&devno, minor, 1, HELLO_DEVICE_NODE_NAME);
        major = MAJOR(devno);
    }
    if(ret<0)
    {
        printk(KERN_ERR HELLO_DEVICE_NODE_NAME " cannot get major %d \n", major);
        return -1;
    }

    printk(KERN_ERR HELLO_DEVICE_NODE_NAME " get major %d \n", major);

    /*注册设备*/
    hello_dev = cdev_alloc();
    if(hello_dev!=NULL)
    {
        /*初始化设备*/
        cdev_init(hello_dev,&hello_cdev_fops);
        hello_dev->owner = THIS_MODULE;
        /*添加设备*/
        if(cdev_add(hello_dev,devno,1)!=0)
        {
            printk(KERN_ERR HELLO_DEVICE_NODE_NAME " add cdev error!\n");
            goto error;
        }
    }
    else
    {
        printk(KERN_ERR HELLO_DEVICE_NODE_NAME " cdev_alloc!\n");
        return -1;
    }

    /*在/sys/class/下创建hello_dev_class目录*/
    hello_dev_class = class_create(THIS_MODULE,HELLO_DEVICE_CLASS_NAME);
    if(IS_ERR(hello_dev_class))
    {
        printk(KERN_INFO HELLO_DEVICE_NODE_NAME " create class error\n");
        return -1;
    }

    /*在/dev下创建hello_dev文件*/
    device_create(hello_dev_class,NULL,devno,NULL,HELLO_DEVICE_FILE_NAME);

    return 0;

    error:
        /*释放已经获得的设备号*/
        unregister_chrdev_region(devno, 1);
        return ret;
}

static void __exit hello_dev_exit(void)
{
    printk(KERN_INFO HELLO_DEVICE_NODE_NAME " dev exit\n");
    device_destroy(hello_dev_class,devno);
    class_destroy(hello_dev_class);
    cdev_del(hello_dev);    /*移除设备*/
    unregister_chrdev_region(devno,1); /*释放设备号*/
}

module_init(hello_dev_init);
module_exit(hello_dev_exit);

MODULE_LICENSE("GPL");
