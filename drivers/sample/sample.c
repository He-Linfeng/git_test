#include<linux/init.h>
#include<linux/module.h>
#include"sample.h"


static int __init sample_dev_init(void)
{
	printk(KERN_INFO SAMPLE_DEVICE_NAME " init!\n");
    return 0;
}

static void __exit sample_dev_exit(void)
{
	printk(KERN_INFO SAMPLE_DEVICE_NAME " exit\n");
}

module_init(sample_dev_init);
module_exit(sample_dev_exit);

MODULE_LICENSE("GPL");
