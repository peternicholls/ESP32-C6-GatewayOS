#ifndef CAPABILITY_H
#define CAPABILITY_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

// Capability Layer - High-level abstractions that map to Zigbee clusters
// Provides stable API for common device capabilities (light.on, switch.toggle, etc.)

/**
 * Initialize the capability layer
 */
esp_err_t capability_init(void);

#endif // CAPABILITY_H
