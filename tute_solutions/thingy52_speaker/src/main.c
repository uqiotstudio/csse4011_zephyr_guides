/*
 * Thingy52: Speaker Sample
 *
 * Auth: Wilfred MK
 * Mail: wilfred.mallawa@wdc.com
 * Date: 20/03/2022
 */
#include <zephyr.h>
#include <drivers/gpio.h>
#include <device.h>
#include <drivers/sensor.h>
#include <drivers/regulator.h>
#include <logging/log.h>
#include <drivers/pwm.h>

/* Define logging module */
LOG_MODULE_REGISTER(speaker_sample, LOG_LEVEL_DBG);

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000
/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)

/* PWM */
#define PWM_PIN 		0x1b
#define PERIOD_1		(USEC_PER_SEC / 500U)
#define PERIOD_2 		(USEC_PER_SEC / 800U)
#define P_WIDTH 		1000 //ms

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);

/* Request to turn on the PWR_REG for the speaker */
int set_spk_pwr_on(void)
{
	const struct device *spk_reg = DEVICE_DT_GET(DT_NODELABEL(spk_pwr));
	int rc = 0;

	if (!(rc = device_is_ready(spk_reg))) {
		LOG_ERR("Speaker regulator device not ready");
		return rc;
	}

	/* Initiation success on non-neg return
	 * NOTE: Ideally instead of passing NULL, setup a callback handler
	 * 		 to verify that the reg is enabled.
	 * This assumes, it setup and doesn't actually check.
	 */
	if ((rc = regulator_enable(spk_reg, NULL)) < 0) {
		return rc;
	}

	LOG_INF("Reg init request OK");
	return 0;
}

void main(void)
{
	const struct device *pwm;
	uint32_t period;
	int ret;
	bool turn = false;

	if (!device_is_ready(led0.port) || !device_is_ready(led1.port) ||
		!device_is_ready(led2.port)) {
		return;
	}

	ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE) |
			gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);

	if (ret != 0) {
		return;
	}

	if (set_spk_pwr_on() != 0) {
		LOG_ERR("REG enable failed");
	}

	pwm = device_get_binding(DT_LABEL(DT_NODELABEL(pwm0)));

	if (!device_is_ready(pwm)) {
		LOG_ERR("PWM not ready");
	}

	while (1) {
		ret = gpio_pin_toggle_dt(&led0);
		if (ret < 0) {
			return;
		}

		period = turn ? PERIOD_1: PERIOD_2;
		turn = !turn;

		if (pwm_pin_set_usec(pwm, PWM_PIN, period, P_WIDTH, 0)) {
			LOG_ERR("PWM set fail");
		}

		LOG_INF("Setting PWM - period %d, width: %d", period, P_WIDTH);
		k_msleep(SLEEP_TIME_MS);

		if (pwm_pin_set_usec(pwm, PWM_PIN, period, 0, 0)) {
			LOG_ERR("PWM clear fail");
		}

		ret = gpio_pin_toggle_dt(&led2);
		if (ret < 0) {
			return;
		}
		LOG_INF("Clearing PWM - period %d, width: %d", period, 0);
		k_msleep(SLEEP_TIME_MS);
	}
}
