#include <linux/module.h>  /* Thu vien nay dinh nghia cac macro nhu module_init/module_exit */
#include <linux/fs.h>      /* Thu vien nay dinh nghia cac ham allocate major/minor number */
#include <linux/device.h>  /* Thu vien nay dinh nghia cac ham class_create/device_create */
#include <linux/cdev.h>    /* Thu vien nay dinh nghia cac ham cdev_init/cdev_add */
#include <linux/slab.h>    /* Thu vien nay dinh nghia cac ham kmalloc */
#include <linux/uaccess.h> /* Thu vien nay dinh nghia cac ham copy_to_user/copy_from_user */
#include <linux/gpio.h>

#define DRIVER_AUTHOR "HieuHoangWork"
#define DRIVER_DESC "GPIOs kernel module"

#define NPAGES 1
#define MAX_GPIOS 128

struct m_foo_dev
{
    int gpio_list[MAX_GPIOS];
    int gpio_count;

    int size;
    char *kmalloc_ptr;
    dev_t dev_num;
    struct class *m_class;
    struct cdev m_cdev;
} mdev = {.gpio_count = 0, .size = 0};

/*  Function Prototypes */
static int __init GPIOs_init(void);
static void __exit GPIOs_exit(void);
static int m_open(struct inode *inode, struct file *file);
static int m_close(struct inode *inode, struct file *file);
static ssize_t m_read(struct file *filp, char __user *user_buf, size_t size, loff_t *offset);
static ssize_t m_write(struct file *filp, const char *user_buf, size_t size, loff_t *offset);

static struct file_operations fops =
    {
        .owner = THIS_MODULE,
        .read = m_read,
        .write = m_write,
        .open = m_open,
        .release = m_close,
};

/* This function will be called when we open the Device file */
static int m_open(struct inode *inode, struct file *file)
{
    pr_info("System call open() called...!!!\n");
    return 0;
}

/* This function will be called when we close the Device file */
static int m_close(struct inode *inode, struct file *file)
{
    pr_info("System call close() called...!!!\n");
    return 0;
}

/* This function will be called when we read the Device file */
static ssize_t m_read(struct file *filp, char __user *user_buffer, size_t size, loff_t *offset)
{
    size_t to_read;

    pr_info("System call read() called...!!!\n");

    /* Check size doesn't exceed our mapped area size */
    to_read = (size > mdev.size - *offset) ? (mdev.size - *offset) : size;

    /* Copy from mapped area to user buffer */
    if (copy_to_user(user_buffer, mdev.kmalloc_ptr + *offset, to_read))
        return -EFAULT;

    *offset += to_read;

    return to_read;
}

/* This function will be called when we write the Device file */
static ssize_t m_write(struct file *filp, const char __user *user_buffer, size_t size, loff_t *offset)
{
    size_t to_write;
    unsigned int gpio_number;
    char gpio_mode[4];
    char gpio_value[5];

    pr_info("System call write() called...!!!\n");

    /* Check size doesn't exceed our mapped area size */
    to_write = (size + *offset > NPAGES * PAGE_SIZE) ? (NPAGES * PAGE_SIZE - *offset) : size;

    /* Copy from user buffer to mapped area */
    memset(mdev.kmalloc_ptr, 0, NPAGES * PAGE_SIZE);
    if (copy_from_user(mdev.kmalloc_ptr + *offset, user_buffer, to_write) != 0)
        return -EFAULT;

    sscanf(mdev.kmalloc_ptr, "%d %s %s", &gpio_number, gpio_mode, gpio_value);
    pr_info("Parsed data: GPIO = %d, Mode = %s, Value = %s\n", gpio_number, gpio_mode, gpio_value);

    if (strcmp(gpio_mode, "out") == 0)
    {
        gpio_direction_output(gpio_number, 0);
        if (strcmp(gpio_value, "HIGH") == 0)
        {
            gpio_set_value(gpio_number, 1);
            pr_info("GPIO %d set as output and HIGH\n", gpio_number);
        }
        else if (strcmp(gpio_value, "LOW") == 0)
        {
            gpio_set_value(gpio_number, 0);
            pr_info("GPIO %d set as output and LOW\n", gpio_number);
        }
        else
        {
            pr_err("Invalid GPIO value. Use 'HIGH' or 'LOW'.\n");
            return -EINVAL;
        }
        if (mdev.gpio_count >= MAX_GPIOS)
        {
            pr_err("Reached the maximum number of GPIOs\n");
            return -ENOMEM;
        }
        mdev.gpio_list[mdev.gpio_count++] = gpio_number;
    }
    else if (strcmp(gpio_mode, "in") == 0)
    {
        gpio_direction_input(gpio_number);
        pr_info("GPIO %d set as input\n", gpio_number);
        if (mdev.gpio_count >= MAX_GPIOS)
        {
            pr_err("Reached the maximum number of GPIOs\n");
            return -ENOMEM;
        }
        mdev.gpio_list[mdev.gpio_count++] = gpio_number;
    }
    else
    {
        pr_err("Invalid GPIO mode. Use 'in' or 'out'.\n");
        return -EINVAL;
    }

    *offset += to_write;
    mdev.size = *offset;

    return to_write;
}

static int
    __init
    GPIOs_init(void) /* Constructor */
{
    /* 1. Allocating device number (cat /pro/devices)*/
    if (alloc_chrdev_region(&mdev.dev_num, 0, 1, "m-cdev") < 0)
    {
        pr_err("Failed to alloc chrdev region\n");
        return -1;
    }
    pr_info("Major = %d Minor = %d\n", MAJOR(mdev.dev_num), MINOR(mdev.dev_num));

    /* 02.1 Creating cdev structure */
    cdev_init(&mdev.m_cdev, &fops);

    /* 02.2 Adding character device to the system*/
    if ((cdev_add(&mdev.m_cdev, mdev.dev_num, 1)) < 0)
    {
        pr_err("Cannot add the device to the system\n");
        goto rm_device_numb;
    }

    /* 03. Creating struct class */
    if ((mdev.m_class = class_create(THIS_MODULE, "m_class")) == NULL)
    {
        pr_err("Cannot create the struct class for my device\n");
        goto rm_device_numb;
    }

    /* 04. Creating device*/
    if ((device_create(mdev.m_class, NULL, mdev.dev_num, NULL, "GPIOs_device")) == NULL)
    {
        pr_err("Cannot create my device\n");
        goto rm_class;
    }

    /* 05. Creating Physical memory*/
    if ((mdev.kmalloc_ptr = kmalloc(1024, GFP_KERNEL)) == 0)
    {
        pr_err("Cannot allocate memory in kernel\n");
        goto rm_device;
    }

    pr_info("GPIOs kernel module ready ...\n");
    return 0;

rm_device:
    device_destroy(mdev.m_class, mdev.dev_num);
rm_class:
    class_destroy(mdev.m_class);
rm_device_numb:
    unregister_chrdev_region(mdev.dev_num, 1);
    return -1;
}

static void
    __exit
    GPIOs_exit(void) /* Destructor */
{
    int i;
    for (i = 0; i < mdev.gpio_count; i++)
    {
        gpio_unexport(mdev.gpio_list[i]);
        gpio_free(mdev.gpio_list[i]);
    }
    kfree(mdev.kmalloc_ptr);                    /* 05 */
    device_destroy(mdev.m_class, mdev.dev_num); /* 04 */
    class_destroy(mdev.m_class);                /* 03 */
    cdev_del(&mdev.m_cdev);                     /* 02 */
    unregister_chrdev_region(mdev.dev_num, 1);  /* 01 */

    pr_info("GPIOs kernel module remove ...\n");
    ;
}

module_init(GPIOs_init);
module_exit(GPIOs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);