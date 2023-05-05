#ifndef __LED_MODULE_H__
#define __LED_MODULE_H__

#define GPIO_1_ADDR_BASE 0x4804C000
#define GPIO_1_ADDR_SIZE (0x4804CFFF - 0x4804C000)

#define GPIO_0_ADDR_BASE 0x44E07000
#define GPIO_0_ADDR_SIZE (0x44E07FFF - 0x44E07000)

#define GPIO_OE_OFFSET 0x134
#define GPIO_CLEARDATAOUT_OFFSET 0x190
#define GPIO_SETDATAOUT_OFFSET 0x194

#define LED_1 (1 << 14) /* P8_16  <=> GPIO_1_14 LED */
#define LED_2 (1 << 31) /* P9_13  <=> GPIO_0_31 LED */

#endif /* __LED_MODULE_H__ */