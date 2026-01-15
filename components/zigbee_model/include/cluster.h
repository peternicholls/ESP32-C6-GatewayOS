#ifndef CLUSTER_H
#define CLUSTER_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

// Zigbee Cluster - group of related attributes and commands

typedef struct zigbee_attribute zigbee_attribute_t;

// Common Zigbee cluster IDs
#define CLUSTER_ID_BASIC            0x0000
#define CLUSTER_ID_POWER_CONFIG     0x0001
#define CLUSTER_ID_IDENTIFY         0x0003
#define CLUSTER_ID_GROUPS           0x0004
#define CLUSTER_ID_SCENES           0x0005
#define CLUSTER_ID_ON_OFF           0x0006
#define CLUSTER_ID_LEVEL_CONTROL    0x0008
#define CLUSTER_ID_COLOR_CONTROL    0x0300
#define CLUSTER_ID_TEMP_MEASUREMENT 0x0402
#define CLUSTER_ID_OCCUPANCY_SENSING 0x0406

typedef struct zigbee_cluster {
    uint16_t cluster_id;
    bool is_server;
    zigbee_attribute_t *attributes;
    uint32_t attribute_count;
    struct zigbee_cluster *next;
} zigbee_cluster_t;

/**
 * Create a new cluster
 */
zigbee_cluster_t* cluster_create(uint16_t cluster_id, bool is_server);

/**
 * Add an attribute to a cluster
 */
esp_err_t cluster_add_attribute(zigbee_cluster_t *cluster, zigbee_attribute_t *attribute);

/**
 * Get an attribute from a cluster
 */
zigbee_attribute_t* cluster_get_attribute(zigbee_cluster_t *cluster, uint16_t attribute_id);

/**
 * Delete a cluster
 */
void cluster_delete(zigbee_cluster_t *cluster);

#endif // CLUSTER_H
