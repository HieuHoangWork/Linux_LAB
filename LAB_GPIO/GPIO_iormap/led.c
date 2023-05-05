#include <linux/module.h> /* This module defines functions such as module_init/module_exit */
#include <linux/io.h>     /* This module defines functions such as ioremap/iounmap */
#include "led.h"          /* LED modules */

#define DRIVER_AUTHOR "HieuHoangWork"
#define DRIVER_DESC "LED blink using ioremap"

uint32_t __iomem *base_addr_LED_1;
uint32_t __iomem *base_addr_LED_2;

/* Constructor */
static int __init led_init(void)
{
    base_addr_LED_1 = ioremap(GPIO_1_ADDR_BASE, GPIO_1_ADDR_SIZE);
    if (!base_addr_LED_1)
        return -ENOMEM;

    base_addr_LED_2 = ioremap(GPIO_0_ADDR_BASE, GPIO_0_ADDR_SIZE);
    if (!base_addr_LED_2)
        return -ENOMEM;

    *(base_addr_LED_1 + GPIO_OE_OFFSET / 4) &= ~LED_1;
    *(base_addr_LED_1 + GPIO_SETDATAOUT_OFFSET / 4) |= LED_1;

    *(base_addr_LED_2 + GPIO_OE_OFFSET / 4) &= ~LED_2;
    *(base_addr_LED_2 + GPIO_SETDATAOUT_OFFSET / 4) |= LED_2;

    pr_info("Hello! Initizliaze successfully!\n");
    return 0;
}

/* Destructor */
static void __exit led_exit(void)
{
    *(base_addr_LED_1 + GPIO_CLEARDATAOUT_OFFSET / 4) |= LED_1;
    iounmap(base_addr_LED_1);

    *(base_addr_LED_2 + GPIO_CLEARDATAOUT_OFFSET / 4) |= LED_2;
    iounmap(base_addr_LED_2);

    pr_info("Good bye my fen !!!\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("1.0");