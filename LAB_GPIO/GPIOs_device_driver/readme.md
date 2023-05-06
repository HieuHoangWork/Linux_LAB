>>Running

1. Change to root permissions
2. insmod gpio.ko
3. echo "48 out HIGH" > /dev/GPIOs_device 

    48: GPIO_pin
    out: GPIO_direction
    HIGH: GPIO_value

GPIO_direction
out: output
in: input

GPIO_value
HIGH: 1
LOW: 0

