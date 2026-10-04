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
#include <our_drivers/our_sensorled.h>

#define DT_DRV_COMPAT our_sensorled

LOG_MODULE_REGISTER(our_sensorled, LOG_LEVEL_INF);

struct sensorled_config {
    struct gpio_dt_spec led_gpio;
};

struct sensorled_data {
    uint32_t fetch_count;
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
    struct sensorled_data *data = dev->data;
    int ret;

    ret = gpio_pin_set_dt(&config->led_gpio, 1);
    LOG_INF("sensorled_sample_fetch(%d) count %d, turn led on returned %d",
            chan, data->fetch_count, ret);
    data->fetch_count++;

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


/*
 * Function to check that the device passed to us is one of ours.
 * We do this by checking the dev->api matches.
 */
static int our_sensorled_check_dev(const struct device *dev)
{
    if (!dev || (dev->api != (void *) &api_sensorled))
      return -EINVAL;
    return 0;
}

int our_sensorled_get_count(const struct device *dev, uint32_t *value)
{
    struct sensorled_data *data = dev->data;
    int check_dev;

    check_dev = our_sensorled_check_dev(dev);

    if (check_dev)
        return check_dev;
    if (!value)
        return -EINVAL;

    *value = data->fetch_count;

    return 0;
}

int our_sensorled_set_count(const struct device *dev, uint32_t value)
{
    struct sensorled_data *data = dev->data;
    int check_dev;

    check_dev = our_sensorled_check_dev(dev);

    if (check_dev)
        return check_dev;

    data->fetch_count = value;

    return 0;
}


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

