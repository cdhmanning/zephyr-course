/*
 * Simple get/set interface of a counter that gets incremented for each fetch.
 *
 */

#ifndef OUR_SENSORLED_CUSTOM_H__
#define OUR_SENSORLED_CUSTOM_H__

#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

int our_sensorled_get_count(const struct device *dev, uint32_t *value);
int our_sensorled_set_count(const struct device *dev, uint32_t value);

#ifdef __cplusplus
}
#endif

#endif
