#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h> // kmalloc
#include <linux/proc_fs.h>
#include <linux/device.h>
#include "hello.h"

static dev_t hello_devno;     /*设备编号*/
static int hello_major = 232; /*主设备号*/
static int hello_minor = 0;   /*从设备号*/

static struct class *hello_class;  /*sysfs结构*/
static struct char_dev *hello_dev; /*字符设备*/

module_param(hello_major, int, S_IRUGO);
module_param(hello_minor, int, S_IRUGO);

static int hello_open(struct inode *inode, struct file *filp)
{
    struct char_dev *dev;
    if (try_module_get(THIS_MODULE))
    {
        dev = container_of(inode->i_cdev, struct char_dev, dev);
        filp->private_data = dev;
        printk(KERN_INFO HELLO_DEVICE_NODE_NAME " open successful!\n");
        return 0;
    }
    else
    {
        printk(KERN_INFO HELLO_DEVICE_NODE_NAME " open failed!\n");
        return -1;
    }
}

static int hello_release(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO HELLO_DEVICE_NODE_NAME " close successful!\n");
    module_put(THIS_MODULE);
    return 0;
}

static ssize_t hello_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
    ssize_t err = 0;
    struct char_dev *dev = filp->private_data;

    if (down_interruptible(&(dev->sem)))
    {
        return -ERESTARTSYS;
    }

    if (count < sizeof(dev->val))
    {
        goto out;
    }

    printk(KERN_INFO HELLO_DEVICE_NODE_NAME " read method!\n");

    err = sizeof(dev->val);

out:
    up(&(dev->sem));
    return err;
}

static ssize_t hello_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
    ssize_t err = 0;
    struct char_dev *dev = filp->private_data;

    if (down_interruptible(&(dev->sem)))
    {
        return -ERESTARTSYS;
    }

    if (count != sizeof(dev->val))
    {
        goto out;
    }

    if (copy_from_user(&(dev->val), buf, count))
    {
        err = -EFAULT;
        goto out;
    }

    printk(KERN_INFO HELLO_DEVICE_NODE_NAME " write method!\n");

    err = sizeof(dev->val);
out:
    up(&(dev->sem));
    return err;
}

static ssize_t __hello_get_val(struct char_dev *dev, char *buf)
{
    int val = 0;

    if (down_interruptible(&(dev->sem)))
    {
        return -ERESTARTSYS;
    }

    val = dev->val;
    up(&(dev->sem));

    return snprintf(buf, PAGE_SIZE, "%d\n", val);
}

static ssize_t __hello_set_val(struct char_dev *dev, const char *buf, size_t count)
{
    int val = 0;
    val = simple_strtol(buf, NULL, 10);
    if (down_interruptible(&(dev->sem)))
    {
        return -ERESTARTSYS;
    }

    dev->val = val;
    up(&(dev->sem));

    return count;
}

static ssize_t hello_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct char_dev *hdev = (struct char_dev *)dev_get_drvdata(dev);
    return __hello_get_val(hdev, buf);
}

static ssize_t hello_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    struct char_dev *hdev = (struct char_dev *)dev_get_drvdata(dev);
    return __hello_set_val(hdev, buf, count);
}

static DEVICE_ATTR(hello, S_IRUGO | S_IWUSR, hello_show, hello_store);

struct file_operations hello_fops = {
    .owner = THIS_MODULE,
    .read = hello_read,
    .write = hello_write,
    .open = hello_open,
    .release = hello_release,
};

static int __hello_setup_dev(struct char_dev *dev)
{
    int err;

    memset(dev, 0, sizeof(struct char_dev));

    cdev_init(&(dev->dev), &hello_fops);
    dev->dev.owner = THIS_MODULE;

    err = cdev_add(&(dev->dev), hello_devno, 1);
    if (err)
    {
        return err;
    }

    sema_init(&(dev->sem), 1);
    dev->val = 0;

    return 0;
}

static int __init hello_init(void)
{
    int err = -1;
    struct device *temp = NULL;
    /*获取设备编号*/
    if (hello_major > 0)
    {
        /*静态获取设备号*/
        hello_devno = MKDEV(hello_major, hello_minor);
        err = register_chrdev_region(hello_devno, 1, HELLO_DEVICE_NODE_NAME);
    }
    else
    {
        /*动态获取设备号*/
        hello_devno = MKDEV(hello_major, hello_minor);
        err = alloc_chrdev_region(&hello_devno, hello_minor, 1, HELLO_DEVICE_NODE_NAME);
        hello_major = MAJOR(hello_devno);
        hello_minor = MINOR(hello_minor);
    }
    if (err < 0)
    {
        printk(KERN_ALERT "Failed to alloc hello_dev region.\n");
        goto fail;
    }

    hello_dev = kmalloc(sizeof(struct char_dev), GFP_KERNEL);
    if (!hello_dev)
    {
        err = -ENOMEM;
        printk(KERN_ALERT "Failed to alloc hell_dev.\n");
        goto unregister;
    }

    err = __hello_setup_dev(hello_dev);
    if (err)
    {
        printk(KERN_ALERT "Failed to setup dev: %d.\n", err);
        goto cleanup;
    }

    /*在/sys/class/下创建hello目录*/
    hello_class = class_create(THIS_MODULE, HELLO_DEVICE_CLASS_NAME);
    if (IS_ERR(hello_class))
    {
        err = PTR_ERR(hello_class);
        printk(KERN_ALERT "Failed to create hello class.\n");
        goto destroy_cdev;
    }

    /*在/dev下创建hello_dev文件*/
    temp = device_create(hello_class, NULL, hello_devno, NULL, HELLO_DEVICE_FILE_NAME);
    if (IS_ERR(temp))
    {
        err = PTR_ERR(temp);
        printk(KERN_ALERT "Failed to create hello device.\n");
        goto destroy_class;
    }

    err = device_create_file(temp, &dev_attr_hello);
    if (err < 0)
    {
        printk(KERN_ALERT "Failed to create attribute val.\n");
        goto destroy_device;
    }

    printk(KERN_ALERT "Successed to initialize hello device.\n");

    return 0;

destroy_device:
    device_destroy(hello_class, hello_devno);

destroy_class:
    class_destroy(hello_class);

destroy_cdev:
    cdev_del(&(hello_dev->dev));

cleanup:
    kfree(hello_dev);

unregister:
    /*释放已经获得的设备号*/
    unregister_chrdev_region(hello_devno, 1);

fail:
    return err;
}

static void __exit hello_exit(void)
{
    printk(KERN_ALERT "Destroy hello device.\n");

    if (hello_class)
    {
        device_destroy(hello_class, hello_devno);
        class_destroy(hello_class);
    }

    if (hello_dev)
    {
        cdev_del(&(hello_dev->dev));
        kfree(hello_dev);
    }

    unregister_chrdev_region(hello_devno, 1); /*释放设备号*/
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
