#ifndef _HELLO_H_
#define _HELLO_H_

#include <linux/cdev.h>
#include <linux/semaphore.h>

#define HELLO_DEVICE_NODE_NAME "hello"
#define HELLO_DEVICE_FILE_NAME "hello"
#define HELLO_DEVICE_PROC_NAME "hello"
#define HELLO_DEVICE_CLASS_NAME "hello"

struct char_dev
{
    int val;              /*寄存器*/
    struct cdev dev;     /*cdev数据结构*/
    struct semaphore sem; /*信号量*/
};
#endif
