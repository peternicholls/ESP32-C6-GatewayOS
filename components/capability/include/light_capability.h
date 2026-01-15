#ifndef LIGHT_CAPABILITY_H
#define LIGHT_CAPABILITY_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

// Light Capability - Maps to Zigbee On/Off, Level Control, and Color Control clusters

typedef struct {
    uint64_t device_addr;
    uint8_t endpoint_id;
} light_device_t;

/**
 * Turn light on
 */
esp_err_t light_on(light_device_t *light);

/**
 * Turn light off
 */
esp_err_t light_off(light_device_t *light);

/**
 * Toggle light state
 */
esp_err_t light_toggle(light_device_t *light);

/**
 * Set light brightness (0-255)
 */
esp_err_t light_set_brightness(light_device_t *light, uint8_t brightness);

/**
 * Set light color temperature (in mireds)
 */
esp_err_t light_set_color_temp(light_device_t *light, uint16_t color_temp);

/**
 * Set light color (HSV)
 */
esp_err_t light_set_color_hsv(light_device_t *light, uint8_t hue, uint8_t saturation, uint8_t value);

/**
 * Get light state
 */
bool light_get_state(light_device_t *light);

#endif // LIGHT_CAPABILITY_H
