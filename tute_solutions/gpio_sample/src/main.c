/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   200

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
#define LED0	DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN	DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS	DT_GPIO_FLAGS(LED0_NODE, gpios)
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led0 devicetree alias is not defined"
#define LED0	""
#define PIN	0
#define FLAGS	0
#endif

/* DeviceTree get node ID from label */
#define GPIO0 DT_NODELABEL(gpio0)
#define GPIO0_13 0x0D //PIN PO.13

void main(void)
{
	const struct device *dev;
	const struct device *dev_gpio0;
	bool led_is_on = true;
	int ret;

	dev = device_get_binding(LED0);
	/* Get device struct for gpio0 */
	dev_gpio0 = device_get_binding(DT_LABEL(GPIO0));

	if (dev == NULL || dev_gpio0 == NULL) {
		return;
	}

	ret = gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);
	if (ret < 0) {
		return;
	}

	/* Configure PIN_A0 as an Output with that is Active Low */
	ret = gpio_pin_configure(dev_gpio0, GPIO0_13, GPIO_OUTPUT_ACTIVE | GPIO_ACTIVE_LOW);

	if (ret < 0) {
		return;
	}

	while (1) {
		gpio_pin_set(dev, PIN, (int)led_is_on);
		/* Toggle the PIN */
		gpio_pin_set(dev_gpio0, GPIO0_13, (int)!led_is_on);
		led_is_on = !led_is_on;
		k_msleep(SLEEP_TIME_MS);
	}
}
