#include "endpoint.h"
#include "cluster.h"
#include "logger.h"
#include <stdlib.h>
#include <string.h>

static const char *TAG = "ENDPOINT";

zigbee_endpoint_t* endpoint_create(uint8_t endpoint_id, uint16_t profile_id, uint16_t device_id)
{
    zigbee_endpoint_t *endpoint = malloc(sizeof(zigbee_endpoint_t));
    if (!endpoint) {
        TINY_LOG_E(TAG, "Failed to allocate endpoint");
        return NULL;
    }
    
    memset(endpoint, 0, sizeof(zigbee_endpoint_t));
    endpoint->endpoint_id = endpoint_id;
    endpoint->profile_id = profile_id;
    endpoint->device_id = device_id;
    endpoint->clusters = NULL;
    endpoint->cluster_count = 0;
    endpoint->next = NULL;
    
    TINY_LOG_D(TAG, "Created endpoint: id=%d, profile=0x%04x, device=0x%04x",
               endpoint_id, profile_id, device_id);
    
    return endpoint;
}

esp_err_t endpoint_add_cluster(zigbee_endpoint_t *endpoint, zigbee_cluster_t *cluster)
{
    if (!endpoint || !cluster) {
        return ESP_ERR_INVALID_ARG;
    }
    
    if (endpoint->clusters == NULL) {
        endpoint->clusters = cluster;
    } else {
        zigbee_cluster_t *last = endpoint->clusters;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = cluster;
    }
    
    endpoint->cluster_count++;
    TINY_LOG_D(TAG, "Added cluster 0x%04x to endpoint %d", 
               cluster->cluster_id, endpoint->endpoint_id);
    
    return ESP_OK;
}

zigbee_cluster_t* endpoint_get_cluster(zigbee_endpoint_t *endpoint, uint16_t cluster_id)
{
    if (!endpoint) {
        return NULL;
    }
    
    zigbee_cluster_t *cluster = endpoint->clusters;
    while (cluster != NULL) {
        if (cluster->cluster_id == cluster_id) {
            return cluster;
        }
        cluster = cluster->next;
    }
    
    return NULL;
}

void endpoint_delete(zigbee_endpoint_t *endpoint)
{
    if (!endpoint) {
        return;
    }
    
    // Delete all clusters
    zigbee_cluster_t *cluster = endpoint->clusters;
    while (cluster != NULL) {
        zigbee_cluster_t *next = cluster->next;
        cluster_delete(cluster);
        cluster = next;
    }
    
    free(endpoint);
    TINY_LOG_D(TAG, "Deleted endpoint");
}
