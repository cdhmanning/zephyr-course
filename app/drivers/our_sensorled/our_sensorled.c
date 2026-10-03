/* 
 * Source module for the our.sensorled driver.
 *
 * This uses the sensor API and responds to:
 *  sensor_sample_fetch()
 *  sensor_sample_get()
 *
 * Not really a useful driver. Just shows how to hook up drivers.
 */

#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT our_sensorled

LOG_MODULE_REGISTER(our_sensorled, LOG_LEVEL_INF);

struct sensorled_config {
    struct gpio_dt_spec led_gpio;
};

struct sensorled_data {
    int dummy;
};

static int sensorled_channel_get(const struct device *dev,
                                 enum sensor_channel chan,
                                 struct sensor_value *val)
{
    const struct sensorled_config *config = dev->config;
    int ret;

    ret = gpio_pin_set_dt(&config->led_gpio, 0);
    LOG_INF("sensorled_channel_get(%d) turn led off returned %d", chan, ret);
    return 0;
}

static int sensorled_sample_fetch(const struct device *dev,
                                   enum sensor_channel chan)
{
    const struct sensorled_config *config = dev->config;
    int ret;

    ret = gpio_pin_set_dt(&config->led_gpio, 1);
    LOG_INF("sensorled_sample_fetch(%d) turn led on returned %d", chan, ret);
    return 0;
}

static int sensorled_init(const struct device *dev)
{
    const struct sensorled_config *config = dev->config;
    int ret;

    LOG_INF("sensorled_init()");

    if (!gpio_is_ready_dt(&config->led_gpio)) {
        LOG_INF("sensorled gpio not available");
        return -ENODEV;
    }
    
    ret = gpio_pin_configure_dt(&config->led_gpio, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        LOG_INF("sensorled gpio failed to set as output");
        return ret;
    }

    return 0;
}

static DEVICE_API(sensor, api_sensorled) = {
    .channel_get = sensorled_channel_get,
    .sample_fetch = sensorled_sample_fetch,
};


#define OUR_SENSORLED_DEFINE(inst)                                           \
                                                                             \
    static struct sensorled_data our_sensorled_data_##inst;              \
    static const struct sensorled_config our_sensorled_config_##inst = { \
        .led_gpio = GPIO_DT_SPEC_GET(DT_DRV_INST(inst), gpios),           \
    };                                                                       \
                                                                             \
DEVICE_DT_INST_DEFINE(inst, sensorled_init, NULL,                            \
                      &our_sensorled_data_##inst,                            \
                      &our_sensorled_config_##inst,                          \
                      POST_KERNEL, 80, &api_sensorled);


DT_INST_FOREACH_STATUS_OKAY(OUR_SENSORLED_DEFINE)

