/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/sensor.h>
#include <sys/printk.h>
#include <drivers/sensor/ccs811.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

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

static bool app_fw_2;

static const char *now_str(void)
{
	static char buf[16]; /* ...HH:MM:SS.MMM */
	uint32_t now = k_uptime_get_32();
	unsigned int ms = now % MSEC_PER_SEC;
	unsigned int s;
	unsigned int min;
	unsigned int h;

	now /= MSEC_PER_SEC;
	s = now % 60U;
	now /= 60U;
	min = now % 60U;
	now /= 60U;
	h = now;

	snprintf(buf, sizeof(buf), "%u:%02u:%02u.%03u",
		 h, min, s, ms);
	return buf;
}

static void hts221_process_sample(const struct device *dev)
{
	static unsigned int obs;
	struct sensor_value temp, hum;
	if (sensor_sample_fetch(dev) < 0) {
		printk("Sensor sample update error\n");
		return;
	}

	if (sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp) < 0) {
		printk("Cannot read HTS221 temperature channel\n");
		return;
	}

	if (sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &hum) < 0) {
		printk("Cannot read HTS221 humidity channel\n");
		return;
	}

	++obs;
	printk("Observation:%u\n", obs);

	/* display temperature */
	printk("Temperature:%.1f C\n", sensor_value_to_double(&temp));

	/* display humidity */
	printk("Relative Humidity:%.1f%%\n",
	       sensor_value_to_double(&hum));
}



static const struct device *get_hts221_dev(void) 
{
	const struct device *dev = device_get_binding("HTS221");

	if (dev == NULL) {
		printk("Error: No such device for hts221\n");
		return NULL;
	}

	if (!device_is_ready(dev)) {
		printk("Error: Device \%s\ not ready\n", dev->name);
	}
	return dev;
}

static void  lis2dh12_fetch_and_display(const struct device *sensor)
{
	static unsigned int count;
	struct sensor_value accel[3];
	struct sensor_value temperature;
	const char *overrun = "";
	int rc = sensor_sample_fetch(sensor);

	++count;
	if (rc == -EBADMSG) {
		/* Sample overrun.  Ignore in polled mode. */
		if (IS_ENABLED(CONFIG_LIS2DH_TRIGGER)) {
			overrun = "[OVERRUN] ";
		}
		rc = 0;
	}
	if (rc == 0) {
		rc = sensor_channel_get(sensor,
					SENSOR_CHAN_ACCEL_XYZ,
					accel);
	}
	if (rc < 0) {
		printk("ERROR: Update failed: %d\n", rc);
	} else {
		printk("#%u @ %u ms: %sx %f , y %f , z %f",
		       count, k_uptime_get_32(), overrun,
		       sensor_value_to_double(&accel[0]),
		       sensor_value_to_double(&accel[1]),
		       sensor_value_to_double(&accel[2]));
	}

	if (IS_ENABLED(CONFIG_LIS2DH_MEASURE_TEMPERATURE)) {
		if (rc == 0) {
			rc = sensor_channel_get(sensor, SENSOR_CHAN_DIE_TEMP, &temperature);
			if (rc < 0) {
				printk("\nERROR: Unable to read temperature:%d\n", rc);
			} else {
				printk(", t %f\n", sensor_value_to_double(&temperature));
			}
		}

	} else {
		printk("\n");
	}
}



const struct device *get_lis2dh12_dev(void) {
	const struct device *sensor = device_get_binding("LIS2DH12-ACCEL");

	if (sensor == NULL) {
		printk("No device found\n");
		return NULL;
	}
	if (!device_is_ready(sensor)) {
		printk("Device %s is not ready\n", sensor->name);
		return NULL;
	}

	return sensor;
}

static void lps22hb_process_sample(const struct device *dev)
{
	static unsigned int obs;
	struct sensor_value pressure, temp;

	if (sensor_sample_fetch(dev) < 0) {
		printk("Sensor sample update error\n");
		return;
	}

	if (sensor_channel_get(dev, SENSOR_CHAN_PRESS, &pressure) < 0) {
		printk("Cannot read LPS22HB pressure channel\n");
		return;
	}

	if (sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp) < 0) {
		printk("Cannot read LPS22HB temperature channel\n");
		return;
	}

	++obs;
	printk("Observation:%u\n", obs);

	/* display pressure */
	printk("Pressure:%.1f kPa\n", sensor_value_to_double(&pressure));

	/* display temperature */
	printk("Temperature:%.1f C\n", sensor_value_to_double(&temp));

}


const struct device *get_lps22hb_dev(void) {
        const struct device *sensor = device_get_binding("LPS22HB");

        if (sensor == NULL) {
                printk("No device found\n");
                return NULL;
        }
        if (!device_is_ready(sensor)) {
                printk("Device %s is not ready\n", sensor->name);
                return NULL;
        }

        return sensor;
}

const struct device *get_ccs811_dev(void) {
        const struct device *sensor = device_get_binding("CCS811");

        if (sensor == NULL) {
                printk("No device found\n");
                return NULL;
        }
        if (!device_is_ready(sensor)) {
                printk("Device %s is not ready\n", sensor->name);
                return NULL;
        }

        return sensor;
}

static int ccs811_process_sample(const struct device *dev)
{
	struct sensor_value co2, tvoc, voltage, current;
	int rc = 0;
	int baseline = -1;

	rc = ccs811_baseline_fetch(dev);

	if (rc >= 0) {
		baseline = rc;
		rc = 0;
	}

	if (rc == 0) {
		rc = sensor_sample_fetch(dev);
	}
	if (rc == 0) {
		const struct ccs811_result_type *rp = ccs811_result(dev);

		sensor_channel_get(dev, SENSOR_CHAN_CO2, &co2);
		sensor_channel_get(dev, SENSOR_CHAN_VOC, &tvoc);
		sensor_channel_get(dev, SENSOR_CHAN_VOLTAGE, &voltage);
		sensor_channel_get(dev, SENSOR_CHAN_CURRENT, &current);

		printk("\n[%s]: CCS811: %u ppm eCO2; %u ppb eTVOC\n",
		       now_str(), co2.val1, tvoc.val1);
		printk("Voltage: %d.%06dV; Current: %d.%06dA\n", voltage.val1,
		       voltage.val2, current.val1, current.val2);
		//printk("BASELINE %04x\n", baseline);

		if (app_fw_2 && !(rp->status & CCS811_STATUS_DATA_READY)) {
			printk("STALE DATA\n");
		}

		if (rp->status & CCS811_STATUS_ERROR) {
			printk("ERROR: %02x\n", rp->error);
		}
	}
	return rc;
}



void main(void)
{
	const struct device *dev;
	bool led_is_on = true;
	int ret;

	dev = device_get_binding(LED0);
	if (dev == NULL) {
		return;
	}

	ret = gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);
	if (ret < 0) {
		return;
	}

	const struct device *hts221_dev = get_hts221_dev();
	const struct device *lis2d12_dev = get_lis2dh12_dev();
	const struct device *lps22hb_dev = get_lps22hb_dev();
	const struct device *ccs811_dev = get_ccs811_dev();

	if (!hts221_dev || !lis2d12_dev || !lps22hb_dev || !ccs811_dev) {
		printk("No device(s) found, exiting\n");
		return ENODEV;
	}

	while (1) {
		gpio_pin_set(dev, PIN, (int)led_is_on);
		led_is_on = !led_is_on;
		k_msleep(SLEEP_TIME_MS);
		printk("-------------------------\n");
		hts221_process_sample(hts221_dev);
		printk("\n");
		lis2dh12_fetch_and_display(lis2d12_dev);
		printk("\n");
		lps22hb_process_sample(lps22hb_dev);
		printk("\n");
		ccs811_process_sample(ccs811_dev);
		printk("-------------------------\n");

	}
}
