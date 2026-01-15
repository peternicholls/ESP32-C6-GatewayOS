#ifndef SWITCH_CAPABILITY_H
#define SWITCH_CAPABILITY_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

// Switch Capability - Maps to Zigbee On/Off cluster

typedef struct {
    uint64_t device_addr;
    uint8_t endpoint_id;
} switch_device_t;

/**
 * Turn switch on
 */
esp_err_t switch_on(switch_device_t *sw);

/**
 * Turn switch off
 */
esp_err_t switch_off(switch_device_t *sw);

/**
 * Toggle switch state
 */
esp_err_t switch_toggle(switch_device_t *sw);

/**
 * Get switch state
 */
bool switch_get_state(switch_device_t *sw);

#endif // SWITCH_CAPABILITY_H
