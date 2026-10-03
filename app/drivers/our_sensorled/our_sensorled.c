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
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT our_sensorled

LOG_MODULE_REGISTER(our_sensorled, LOG_LEVEL_INF);

static int sensorled_channel_get(const struct device *dev,
                                 enum sensor_channel chan,
                                 struct sensor_value *val)
{
    LOG_INF("sensorled_channel_get(%d)", chan);
    return 0;
}

static int sensorled_sample_fetch(const struct device *dev,
                                   enum sensor_channel chan)
{
    LOG_INF("sensorled_sample_fetch(%d)", chan);
    return 0;
}

static int sensorled_init(const struct device *dev)
{
    LOG_INF("sensorled_init()");
    return 0;
}

static DEVICE_API(sensor, api_sensorled) = {
    .channel_get = sensorled_channel_get,
    .sample_fetch = sensorled_sample_fetch,
};

DEVICE_DT_INST_DEFINE(0, sensorled_init, NULL, NULL, NULL, POST_KERNEL, 80, &api_sensorled);


#define DT_DRV_COMPAT our_sensorled
