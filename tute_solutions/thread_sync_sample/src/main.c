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
#define STACK_SIZE 256
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


/* Define Semaphores */
K_SEM_DEFINE(on_sem, 0, 1);
K_SEM_DEFINE(off_sem, 1, 1);

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
		/* Wait to be indicated that the led is on by thread2 */
		if (k_sem_take(&on_sem, K_SECONDS(10)) != 0) {
			/*
			 * Some logic for fallback, we coudn't get the semaphore within
			 * 	the specified timeout. 
			 */
		}

		/* Note: The delays are implemented so that the led is visibly blinking
		 * But the two threads are synchronised by the semaphores, where they operate one
		 * after the other, in context of led on, led off. 
		 */
		k_msleep(500);
		gpio_pin_set(dev, PIN, led_stat);
		k_msleep(500);
		k_sem_give(&off_sem);
	}

}


/* 
 * Thread to configure GPIO for led0 and to turn the led on. This thread
 * 	will run first (started without a scheduling delay)
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
		/* Wait forever, till off sem ready
		 * Note: off_sem start off with a count of one
		 */
		k_sem_take(&off_sem, K_FOREVER);

		gpio_pin_set(dev, PIN, (int)led_stat);
		k_sem_give(&on_sem);
	}
}

