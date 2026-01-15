#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

// Zigbee Endpoint - logical sub-device on a Zigbee node

typedef struct zigbee_cluster zigbee_cluster_t;

typedef struct zigbee_endpoint {
    uint8_t endpoint_id;
    uint16_t profile_id;
    uint16_t device_id;
    zigbee_cluster_t *clusters;
    uint32_t cluster_count;
    struct zigbee_endpoint *next;
} zigbee_endpoint_t;

/**
 * Create a new endpoint
 */
zigbee_endpoint_t* endpoint_create(uint8_t endpoint_id, uint16_t profile_id, uint16_t device_id);

/**
 * Add a cluster to an endpoint
 */
esp_err_t endpoint_add_cluster(zigbee_endpoint_t *endpoint, zigbee_cluster_t *cluster);

/**
 * Get a cluster from an endpoint
 */
zigbee_cluster_t* endpoint_get_cluster(zigbee_endpoint_t *endpoint, uint16_t cluster_id);

/**
 * Delete an endpoint
 */
void endpoint_delete(zigbee_endpoint_t *endpoint);

#endif // ENDPOINT_H
