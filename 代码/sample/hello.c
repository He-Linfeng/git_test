#include<linux/init.h>
#include<linux/module.h>
#include"hello.h"


static int __init hello_dev_init(void)
{
	printk(KERN_ERR HELLO_DEVICE_NAME " init!\n");
    return 0;
}

static void __exit hello_dev_exit(void)
{
	printk(KERN_INFO HELLO_DEVICE_NAME " exit\n");
}

module_init(hello_dev_init);
module_exit(hello_dev_exit);

MODULE_LICENSE("GPL");
