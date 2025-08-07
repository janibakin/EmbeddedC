#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/cdev.h>

#define DEVICE_NAME "stackdev"
#define STACK_SIZE 16

static int stack[STACK_SIZE];
static int top = -1; // top element index, -1 to indicate an empty stack
static dev_t dev_num;
static struct cdev stack_cdev;

// open device
static int stack_open(struct inode *inode, struct file *file) {
    pr_info("stackdev: device opened\n");
    return 0;
}

// release/close device
static int stack_release(struct inode *inode, struct file *file) {
    pr_info("stackdev: device closed\n");
    return 0;
}

// write (push)
static ssize_t stack_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
    int value;

    if (top >= STACK_SIZE - 1) {
        pr_err("stackdev: stack overflow\n");
        return -ENOMEM; // stack overflow
    }

    if (copy_from_user(&value, buf, sizeof(int))) {
        pr_err("stackdev: failed to copy data from user\n");
        return -EFAULT;
    }

    stack[++top] = value; // add to stack
    pr_info("stackdev: pushed %d to stack\n", value);
    return sizeof(int);
}

// read (pop)
static ssize_t stack_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
    int value;

    if (top < 0) {
        pr_err("stackdev: stack underflow\n");
        return -EINVAL; // empty stack
    }

    value = stack[top--]; // remove from stack

    if (copy_to_user(buf, &value, sizeof(int))) {
        pr_err("stackdev: failed to copy data to user\n");
        return -EFAULT;
    }

    pr_info("stackdev: popped %d from stack\n", value);
    return sizeof(int);
}

// file operations structure
static const struct file_operations stack_fops = {
    .owner = THIS_MODULE,
    .open = stack_open,
    .release = stack_release,
    .write = stack_write,
    .read = stack_read,
};

// Module initialisation
static int __init stack_init(void) {
    int ret;

    // Register character device number
    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_err("stackdev: failed to allocate device number\n");
        return ret;
    }

    // Initialise character device
    cdev_init(&stack_cdev, &stack_fops);
    stack_cdev.owner = THIS_MODULE;

    ret = cdev_add(&stack_cdev, dev_num, 1);
    if (ret < 0) {
        pr_err("stackdev: failed to add cdev\n");
        unregister_chrdev_region(dev_num, 1);
        return ret;
    }

    pr_info("stackdev: module loaded, device number %d:%d\n", MAJOR(dev_num), MINOR(dev_num));
    return 0;
}

// Unregister character device and clean up
static void __exit stack_exit(void) {
    cdev_del(&stack_cdev);
    unregister_chrdev_region(dev_num, 1);
    pr_info("stackdev: module unloaded\n");
}

module_init(stack_init);
module_exit(stack_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Zhanibek Bakin");
MODULE_DESCRIPTION("Simple stack device driver");