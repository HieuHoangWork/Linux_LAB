#include <linux/module.h>  /* This module defines macro such as module_init/module_exit */
#include <linux/fs.h>      /* This module defines functions such as allocate major/minor number */
#include <linux/device.h>  /* This module defines functions such as class_create/device_create */
#include <linux/cdev.h>    /* This module defines functions such as kmalloc */
#include <linux/slab.h>    /* This module defines functions such as cdev_init/cdev_add */
#include <linux/uaccess.h> /* This module defines functions such as copy_to_user/copy_from_user */
#include <linux/string.h>  /* This module defines functions such as strncpy/strcmp */
#include <linux/gpio.h>    /* This module defines functions such as gpio_request/gpio_set_value */
#include "bbb-sysfs.h"

static unsigned int GPIO_LED_1 = 46;
static unsigned int GPIO_LED_2 = 31;

static int32_t _value = 0;
static char _direct[8] = {};

struct m_foo_dev
{
    struct kobject *kobj_ref;
} mdev;

/*************** Function Prototypes *******************/
static int __init gpio_sysfs_init(void);
static void __exit gpio_sysfs_exit(void);

/***************** Sysfs functions *******************/
static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);
static ssize_t direction_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t direction_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);

struct kobj_attribute value = __ATTR(value, 0660, value_show, value_store);
struct kobj_attribute direction = __ATTR(direction, 0660, direction_show, direction_store);

static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", _value);
}

static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    int32_t numb = 0;
    sscanf(buf, "%d", &numb);

    switch (numb)
    {
    case 0: /* off */
        gpio_set_value(GPIO_LED_1, 0);
        gpio_set_value(GPIO_LED_2, 0);
        pr_info("LED OFF\n");
        break;
    case 1: /* on */
        gpio_set_value(GPIO_LED_1, 1);
        gpio_set_value(GPIO_LED_2, 1);
        pr_info("LED ON\n");
        break;

    default:
        return count;
    }
    sscanf(buf, "%d", &_value);

    return count;
}

static ssize_t direction_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%s", _direct);
}

static ssize_t direction_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    int8_t check_buf;
    static const char s_directions_str[] = "in\0out";
    strcpy(_direct, buf);
    if (strncmp(buf, &s_directions_str[0], 2) == 0)
    {
        check_buf = 0;
        pr_info("LED IN\n");
    }
    else if (strncmp(buf, &s_directions_str[3], 3) == 0)
    {
        check_buf = 1;
        pr_info("LED OUT\n");
    }
    switch (check_buf)
    {
    case 0: /* in */
        gpio_direction_input(GPIO_LED_1);
        gpio_direction_input(GPIO_LED_2);
        pr_info("LED SET IN\n");
        break;
    case 1: /* out */
        gpio_direction_output(GPIO_LED_1, 0);
        gpio_direction_output(GPIO_LED_2, 0);
        pr_info("LED SET OUT\n");
        break;

    default:
        return count;
    }

    return count;
}

/* Initialize group attrs */
static struct attribute *attrs[] = {
    &direction.attr,
    &value.attr,
    NULL,
};

static struct attribute_group attr_group = {
    .attrs = attrs,
};

static int
    __init
    gpio_sysfs_init(void) /* Constructor */
{

    /* 01. It will create a directory under "/sys" , [firmware_kobj, class_kobj] */
    mdev.kobj_ref = kobject_create_and_add("bbb_gpios", NULL);

    /* 02. Creating group sys entry under "/sys/bbb_gpios/" */
    if (sysfs_create_group(mdev.kobj_ref, &attr_group))
    {
        pr_err("Cannot create group atrributes......\n");
        goto rm_kobj;
    }

    /* 03. Request GPIO */
    gpio_request(GPIO_LED_1, "led_1");
    gpio_request(GPIO_LED_2, "led_2");

    pr_info("Hello! Initizliaze successfully!!\n");
    return 0;

rm_kobj:
    kobject_put(mdev.kobj_ref);
    return -1;
}

static void
    __exit
    gpio_sysfs_exit(void) /* Destructor */
{
    gpio_free(GPIO_LED_1);
    gpio_free(GPIO_LED_2);                          /* 03 */
    sysfs_remove_group(mdev.kobj_ref, &attr_group); /* 02 */
    kobject_put(mdev.kobj_ref);                     /* 01 */

    pr_info("Good bye my fen !!!\n");
}

module_init(gpio_sysfs_init);
module_exit(gpio_sysfs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("1.0");