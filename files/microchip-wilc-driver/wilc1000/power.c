#include <linux/delay.h>
#include <linux/of.h>
#include <linux/version.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>

#include "netdev.h"

/**
 * wilc_of_parse_power_pins() - parse power sequence pins; to keep backward
 *		compatibility with old device trees that doesn't provide
 *		power sequence pins we check for default pins on proper boards
 *
 * @wilc:	wilc data structure
 *
 * Returns:	 0 on success, negative error number on failures.
 */
int wilc_of_parse_power_pins(struct wilc *wilc)
{
	static const struct wilc_power_gpios default_gpios[] = {
		{ .reset = GPIO_NUM_RESET,	.chip_en = GPIO_NUM_CHIP_EN, },
	};
	struct device_node *of = wilc->dt_dev->of_node;
	struct wilc_power *power = &wilc->power;
	const struct wilc_power_gpios *gpios = &default_gpios[0];
	int ret;

	power->gpios.reset = of_get_named_gpio_flags(of, "reset-gpios", 0,
						     NULL);
	if (!gpio_is_valid(power->gpios.reset))
		power->gpios.reset = gpios->reset;

	power->gpios.chip_en = of_get_named_gpio_flags(of, "chip_en-gpios", 0,
						       NULL);
	if (!gpio_is_valid(power->gpios.chip_en))
		power->gpios.chip_en = gpios->chip_en;

	if (!gpio_is_valid(power->gpios.chip_en) ||
			!gpio_is_valid(power->gpios.reset))
		return -EINVAL;

	ret = devm_gpio_request(wilc->dev, power->gpios.chip_en, "CHIP_EN");
	if (ret)
		return ret;

	ret = devm_gpio_request(wilc->dev, power->gpios.reset, "RESET");
	return ret;
}

/**
 * wilc_wlan_power() - handle power on/off commands
 *
 * @wilc:	wilc data structure
 * @on:		requested power status
 *
 * Returns:	none
 */
void wilc_wlan_power(struct wilc *wilc, bool on)
{
	if (!gpio_is_valid(wilc->power.gpios.chip_en) ||
	    !gpio_is_valid(wilc->power.gpios.reset)) {
		/* In case SDIO power sequence driver is used to power this
		 * device then the powering sequence is handled by the bus
		 * via pm_runtime_* functions. */
		return;
	}

	if (on) {
		gpio_direction_output(wilc->power.gpios.chip_en, 1);
		mdelay(5);
		gpio_direction_output(wilc->power.gpios.reset, 1);
	} else {
		gpio_direction_output(wilc->power.gpios.chip_en, 0);
		gpio_direction_output(wilc->power.gpios.reset, 0);
	}
}
