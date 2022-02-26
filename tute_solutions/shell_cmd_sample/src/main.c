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
#include <shell/shell.h>


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

/* Declare command handler prototypes */
static int cmd_led_ctrl_on(const struct shell *, size_t, char **);
static int cmd_led_ctrl_off(const struct shell *, size_t, char **);


/* Specify Shell Commands for LED Toggling */
/* Creating subcommands (level 1 command) array for command "led". */ 
SHELL_STATIC_SUBCMD_SET_CREATE(led_ctrl,
        SHELL_CMD(on, NULL, "Turn led on.", cmd_led_ctrl_on),
        SHELL_CMD(off,   NULL, "Turn led off.", cmd_led_ctrl_off),
        SHELL_SUBCMD_SET_END
); 

/* Creating root (level 0) command "led" */
SHELL_CMD_REGISTER(led, &led_ctrl, "Led Control (On/Off)", NULL);


/* Command Handler for toggling led0 on, note that it assumes
 * the pin has been preconfigured */
static int cmd_led_ctrl_on(const struct shell *shell, size_t argc,
                        char **argv)
{
        ARG_UNUSED(argc);
        ARG_UNUSED(argv);

	const struct device *dev = device_get_binding(LED0);

	if (dev == NULL) {
		return ENODEV;
	}

	return gpio_pin_set(dev, PIN, 1);
}

/* Command Handler for toggling led0 off, note that it assumes
 * the pin has been preconfigured */
static int cmd_led_ctrl_off(const struct shell *shell, size_t argc,
                        char **argv)
{
        ARG_UNUSED(argc);
        ARG_UNUSED(argv);

	const struct device *dev = device_get_binding(LED0);

	if (dev == NULL) {
		return ENODEV;
	}

	return gpio_pin_set(dev, PIN, 0);
}



/*
 * Sample app to init USB, and demonstrate shell functionality
 */
void
main(void)
{
	const struct device *dev;
	int ret;

	/* Setup DTR */
	const struct device *shell_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_shell_uart));
	uint32_t dtr = 0;
	
	if (usb_enable(NULL))
		return;

	/* Wait on DTR - 'Data Terminal Ready'
	 * Will wait here until a terminal has been attached to the device
	 * This is not necessary, however, can be useful from reading early data
	 *
	 * Remove the following while loop if DTR is not required!!
	 */
	
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
}
