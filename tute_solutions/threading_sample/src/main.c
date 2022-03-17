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

/* Thread Definitions */
#define STACK_SIZE 512
#define PRIORITY 3

/* Thread Prototypes */
void thread1_entry(void *, void *, void *);
void thread2_entry(void *, void *, void *);


/* Compile Time Threads */

/*
 * Note that thread1 has a scheduling delay of 100ms, 
 */
K_THREAD_DEFINE(thread1_tid, STACK_SIZE,
		thread1_entry, NULL, NULL, NULL,
		PRIORITY, 0, 100);

K_THREAD_DEFINE(thread2_tid, STACK_SIZE,
		thread2_entry, NULL, NULL, NULL,
		PRIORITY, 0, 0);



/* Note: What is wrong with this implementation? How could you make it better? */

/*
 * Thread to turn the led off
 */
void
thread1_entry(void *arg1, void *arg2, void *arg3)
{
	const struct device *dev;
	bool led_stat = false;
	dev = device_get_binding(LED0);

	if (dev == NULL)
		return;

	while (1)
	{
		k_msleep(600);
		gpio_pin_set(dev, PIN, led_stat);
		k_msleep(600);
	}

}


/* 
 * Thread to configure GPIO for led0 and to turn the led on.
 */
void
thread2_entry(void *arg1, void *arg2, void *arg3)
{
	const struct device *dev;
	bool led_stat = true;
	dev = device_get_binding(LED0);

	if (dev == NULL)
		return;

	if (gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE | FLAGS) < 0)
		return;

	while (1)
	{
		k_msleep(400);
		gpio_pin_set(dev, PIN, (int)led_stat);
		k_msleep(400);
	}
}
