/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <usb/usb_device.h>
#include <drivers/uart.h>
#include <string.h>


/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   500

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

struct data_packet {
	uint16_t preamble;
	char string[32];
};

/* Define a Message Queue */
K_MSGQ_DEFINE(my_msgq, sizeof(struct data_packet), 10, 4);

/* Define Consumer Thread */
void consumer_thread(void);
#define STACK_SIZE 1024
#define THREAD_PRIOR 3
#define THREAD_DELAY 0
#define OPTIONS 0
K_THREAD_DEFINE(consumer_thread_tid, STACK_SIZE, consumer_thread, NULL, NULL, NULL, THREAD_PRIOR, OPTIONS, THREAD_DELAY);

/*
 * Consumer thread for testing message queue 
 */
void
consumer_thread(void) 
{
	struct data_packet data;

	while(1) {
		if (k_msgq_get(&my_msgq, &data, K_FOREVER) == 0) {
			if (data.preamble == 0xAA)
				printk("MSG Received: %s\n", data.string);
			memset(&data, 0, sizeof(struct data_packet));
		}
	}
}

/*
 * Sample app to init USB, and demonstrate shell functionality
 */
void
main(void)
{
	const struct device *dev;
	bool led_is_on = true;
	int ret;

	/* Setup DTR */
	const struct device *shell_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_shell_uart));
	uint32_t dtr = 0;

	/* MSGQ Packet */
	struct data_packet packet;
	packet.preamble = 0xAA;
	snprintk(packet.string, sizeof(packet.string), "Colonels Recipe.exe\n");
	
	if (usb_enable(NULL))
		return;

	/* Wait on DTR - 'Data Terminal Ready'*/
	while (!dtr) {
		uart_line_ctrl_get(shell_dev, UART_LINE_CTRL_DTR, &dtr);
		k_sleep(K_MSEC(100));
	} 

	/* DTR - OK, Continue */
	dev = device_get_binding(LED0);
	if (dev == NULL) {
		return;
	}

	ret = gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);
	if (ret < 0) {
		return;
	}

	while (1) {
		gpio_pin_set(dev, PIN, (int)led_is_on);
		led_is_on = !led_is_on;

		/* Send Messages to consumers */
		if (k_msgq_put(&my_msgq, &packet, K_NO_WAIT) != 0) {
			/* Queue is full, we could purge it, a loop can be
			 * implemented here to keep trying after a purge.
		 	 */
			k_msgq_purge(&my_msgq);
		}
		k_sleep(K_SECONDS(5));
	}
}
