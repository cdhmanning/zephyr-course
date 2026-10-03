#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#define SLEEP_TIME_MS 2000

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static int sensorled_test(void)
{
    const struct device *sensorled = DEVICE_DT_GET(DT_NODELABEL(our_sensorled0));
    struct sensor_value val;
    int ret;

    if (!device_is_ready(sensorled)) {
	LOG_INF("sensorled: device not ready.\n");
	return 0;
    }
    while(1) {
        ret = sensor_sample_fetch(sensorled);
        LOG_INF("sensor_sample_fetch() returned %d", ret);
        k_msleep(SLEEP_TIME_MS);
        ret = sensor_channel_get(sensorled, SENSOR_CHAN_AMBIENT_TEMP, &val);
        LOG_INF("sensor_channel_get() returned %d", ret);
        k_msleep(SLEEP_TIME_MS);
    }

    return 0;
}

int main(void)
{
    return sensorled_test();

#if 0
    bool led_state = true;

    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    while (1) {
        if (gpio_pin_toggle_dt(&led) < 0) return 0;
        k_msleep(SLEEP_TIME_MS/8);
        if (gpio_pin_toggle_dt(&led) < 0) return 0;
        k_msleep(SLEEP_TIME_MS/8);
        if (gpio_pin_toggle_dt(&led) < 0) return 0;
        k_msleep(SLEEP_TIME_MS/8);
        if (gpio_pin_toggle_dt(&led) < 0) return 0;

        led_state = !led_state;
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        k_msleep(SLEEP_TIME_MS);
    }
#endif
    return 0;
}
