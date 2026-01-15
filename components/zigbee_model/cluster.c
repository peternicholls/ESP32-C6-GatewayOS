#include "cluster.h"
#include "attribute.h"
#include "logger.h"
#include <stdlib.h>
#include <string.h>

static const char *TAG = "CLUSTER";

zigbee_cluster_t* cluster_create(uint16_t cluster_id, bool is_server)
{
    zigbee_cluster_t *cluster = malloc(sizeof(zigbee_cluster_t));
    if (!cluster) {
        TINY_LOG_E(TAG, "Failed to allocate cluster");
        return NULL;
    }
    
    memset(cluster, 0, sizeof(zigbee_cluster_t));
    cluster->cluster_id = cluster_id;
    cluster->is_server = is_server;
    cluster->attributes = NULL;
    cluster->attribute_count = 0;
    cluster->next = NULL;
    
    TINY_LOG_D(TAG, "Created cluster: id=0x%04x, %s",
               cluster_id, is_server ? "server" : "client");
    
    return cluster;
}

esp_err_t cluster_add_attribute(zigbee_cluster_t *cluster, zigbee_attribute_t *attribute)
{
    if (!cluster || !attribute) {
        return ESP_ERR_INVALID_ARG;
    }
    
    if (cluster->attributes == NULL) {
        cluster->attributes = attribute;
    } else {
        zigbee_attribute_t *last = cluster->attributes;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = attribute;
    }
    
    cluster->attribute_count++;
    TINY_LOG_D(TAG, "Added attribute 0x%04x to cluster 0x%04x", 
               attribute->attribute_id, cluster->cluster_id);
    
    return ESP_OK;
}

zigbee_attribute_t* cluster_get_attribute(zigbee_cluster_t *cluster, uint16_t attribute_id)
{
    if (!cluster) {
        return NULL;
    }
    
    zigbee_attribute_t *attr = cluster->attributes;
    while (attr != NULL) {
        if (attr->attribute_id == attribute_id) {
            return attr;
        }
        attr = attr->next;
    }
    
    return NULL;
}

void cluster_delete(zigbee_cluster_t *cluster)
{
    if (!cluster) {
        return;
    }
    
    // Delete all attributes
    zigbee_attribute_t *attr = cluster->attributes;
    while (attr != NULL) {
        zigbee_attribute_t *next = attr->next;
        attribute_delete(attr);
        attr = next;
    }
    
    free(cluster);
    TINY_LOG_D(TAG, "Deleted cluster");
}
