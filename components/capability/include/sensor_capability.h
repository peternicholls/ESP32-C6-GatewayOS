#ifndef SENSOR_CAPABILITY_H
#define SENSOR_CAPABILITY_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

// Sensor Capability - Maps to various Zigbee measurement clusters

typedef struct {
    uint64_t device_addr;
    uint8_t endpoint_id;
} sensor_device_t;

/**
 * Get temperature reading (in degrees Celsius)
 */
esp_err_t sensor_get_temperature(sensor_device_t *sensor, float *temperature);

/**
 * Get humidity reading (in percent)
 */
esp_err_t sensor_get_humidity(sensor_device_t *sensor, float *humidity);

/**
 * Get occupancy state
 */
esp_err_t sensor_get_occupancy(sensor_device_t *sensor, bool *occupied);

/**
 * Get battery level (in percent)
 */
esp_err_t sensor_get_battery(sensor_device_t *sensor, uint8_t *battery_percent);

#endif // SENSOR_CAPABILITY_H
