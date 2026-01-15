#ifndef NODE_H
#define NODE_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

// Zigbee Node - represents a Zigbee device

typedef struct zigbee_endpoint zigbee_endpoint_t;

typedef struct zigbee_node {
    uint16_t short_addr;
    uint64_t ieee_addr;
    char *manufacturer;
    char *model;
    bool online;
    zigbee_endpoint_t *endpoints;
    uint32_t endpoint_count;
    struct zigbee_node *next;
} zigbee_node_t;

/**
 * Create a new Zigbee node
 */
zigbee_node_t* node_create(uint16_t short_addr, uint64_t ieee_addr);

/**
 * Add an endpoint to a node
 */
esp_err_t node_add_endpoint(zigbee_node_t *node, zigbee_endpoint_t *endpoint);

/**
 * Get an endpoint from a node
 */
zigbee_endpoint_t* node_get_endpoint(zigbee_node_t *node, uint8_t endpoint_id);

/**
 * Set node online/offline status
 */
void node_set_online(zigbee_node_t *node, bool online);

/**
 * Delete a node
 */
void node_delete(zigbee_node_t *node);

#endif // NODE_H
