#ifndef ZIGBEE_MODEL_H
#define ZIGBEE_MODEL_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

// Zigbee Canonical Model: Node → Endpoint → Cluster → Attribute

/**
 * Initialize the Zigbee model layer
 */
esp_err_t zigbee_model_init(void);

#endif // ZIGBEE_MODEL_H
