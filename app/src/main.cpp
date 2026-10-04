#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <our_drivers/our_sensorled.h>

int our_sensorled_get_count(const struct device *dev, uint32_t *value);
int our_sensorled_set_count(const struct device *dev, uint32_t value);

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
    uint32_t fetch_count;

    if (!device_is_ready(sensorled)) {
	LOG_INF("sensorled: device not ready.\n");
	return 0;
    }
    while(1) {
        ret = our_sensorled_get_count(sensorled, &fetch_count);
        LOG_INF("sensor_get_count ret %d, value %u", ret, fetch_count);
        /*
         *The fetch count counts up every time a fetch is done.
         * We fiddle with the fetch count by setting it, so that it counts from
         * 10 to 20 then hops to 50 to 60 then back again.
         */
        if (fetch_count < 10 || fetch_count >= 60)
            ret = our_sensorled_set_count(sensorled, 10);
        else if (fetch_count >= 20 && fetch_count < 50)
            ret = our_sensorled_set_count(sensorled, 50);
        
        
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
