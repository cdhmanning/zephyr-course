#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

/*
 * The devicetree node identifier for the "app-led" alias. 
 * Note: app-led in dts, app_led in C!
 */
#define LED_NODE_HB DT_ALIAS(app_led)

static const struct gpio_dt_spec heart_led = GPIO_DT_SPEC_GET(LED_NODE_HB, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = true;
    unsigned heart_counter = 0;
    unsigned heart_led_on;

    if (!gpio_is_ready_dt(&heart_led)) return 0;

    if (gpio_pin_configure_dt(&heart_led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    while (1) {
        if ((heart_counter & 7 ) == 0)
            LOG_INF("Heartbeat counter %u", heart_counter);
        /* Create a heartbeat by splitting the period in 8 and turning on the
         * LED during 2 of those sub periods:  *_*_____*_*_____ ...
         */
        heart_led_on = ((heart_counter & 7) == 0 || (heart_counter & 7) == 2);
        if (gpio_pin_set_dt(&heart_led, heart_led_on) < 0)
            return 0;
        heart_counter++;
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS/8);
    }
    return 0;
}
