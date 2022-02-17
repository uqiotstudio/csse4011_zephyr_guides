/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

/* Include file from our library */
#include "led_driver.h"

/*
 * Simple application that implements the led library to init and toggle
 * 	the led0 forever
 */
void main(void)
{
	/* ENODEV */
	if (led0_init() == ENODEV)
		return;

	while (1) {
		led0_on();
		k_sleep(K_SECONDS(2));
		led0_off();
		k_sleep(K_SECONDS(1));
	}
}
