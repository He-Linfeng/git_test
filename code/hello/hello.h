#ifndef _HELLO_ANDROID_H_
#define _HELLO_ANDROID_H_

#include <linux/cdev.h>
#include <linux/semaphore.h>

#define HELLO_DEVICE_NODE_NAME "hello"
#define HELLO_DEVICE_FILE_NAME "hello"
#define HELLO_DEVICE_PROC_NAME "hello"
#define HELLO_DEVICE_CLASS_NAME "hello"

static int major = 232;
static int minor = 0;

struct cdev *hello_dev; /*cdev数据结构*/
static dev_t devno; /*设备编号*/
static struct class *hello_dev_class;


#endif


