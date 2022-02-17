#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
/* Local Library Include */
#include "led_driver.h"

/*
 * Initialize red led gpio pin
 */
int 
led0_init(void)
{
	const struct device *dev;
	
	/* LED0 referenced from led_driver.h */
	dev = device_get_binding(LED0);
	if (dev == NULL)
		return ENODEV;
	return gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);
}

/* 
 * deinitialize red led gpio pin
 */
int
led0_deinit(void) 
{
	const struct device *dev;
	
	dev = device_get_binding(LED0);
	if (dev == NULL)
		return ENODEV;
	
	return gpio_pin_configure(dev, PIN, GPIO_DISCONNECTED);
}

/*
 * Turn red led on
 */
int
led0_on(void)
{
	const struct device *dev;
	
	dev = device_get_binding(LED0);
	if (dev == NULL)
		return ENODEV;
	
	return gpio_pin_set(dev, PIN, 1);
}


/*
 * Turn the red led off 
 */
int
led0_off(void)
{
	const struct device *dev;
	
	dev = device_get_binding(LED0);
	if (dev == NULL)
		return ENODEV;
	
	return gpio_pin_set(dev, PIN, 0);
}



