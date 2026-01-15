#include "node.h"
#include "endpoint.h"
#include "logger.h"
#include <stdlib.h>
#include <string.h>

static const char *TAG = "NODE";

zigbee_node_t* node_create(uint16_t short_addr, uint64_t ieee_addr)
{
    zigbee_node_t *node = malloc(sizeof(zigbee_node_t));
    if (!node) {
        TINY_LOG_E(TAG, "Failed to allocate node");
        return NULL;
    }
    
    memset(node, 0, sizeof(zigbee_node_t));
    node->short_addr = short_addr;
    node->ieee_addr = ieee_addr;
    node->online = false;
    node->endpoints = NULL;
    node->endpoint_count = 0;
    node->next = NULL;
    
    TINY_LOG_I(TAG, "Created node: short_addr=0x%04x, ieee_addr=0x%016llx", 
               short_addr, ieee_addr);
    
    return node;
}

esp_err_t node_add_endpoint(zigbee_node_t *node, zigbee_endpoint_t *endpoint)
{
    if (!node || !endpoint) {
        return ESP_ERR_INVALID_ARG;
    }
    
    if (node->endpoints == NULL) {
        node->endpoints = endpoint;
    } else {
        zigbee_endpoint_t *last = node->endpoints;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = endpoint;
    }
    
    node->endpoint_count++;
    TINY_LOG_D(TAG, "Added endpoint %d to node 0x%04x", 
               endpoint->endpoint_id, node->short_addr);
    
    return ESP_OK;
}

zigbee_endpoint_t* node_get_endpoint(zigbee_node_t *node, uint8_t endpoint_id)
{
    if (!node) {
        return NULL;
    }
    
    zigbee_endpoint_t *ep = node->endpoints;
    while (ep != NULL) {
        if (ep->endpoint_id == endpoint_id) {
            return ep;
        }
        ep = ep->next;
    }
    
    return NULL;
}

void node_set_online(zigbee_node_t *node, bool online)
{
    if (node) {
        node->online = online;
        TINY_LOG_I(TAG, "Node 0x%04x is now %s", 
                   node->short_addr, online ? "online" : "offline");
    }
}

void node_delete(zigbee_node_t *node)
{
    if (!node) {
        return;
    }
    
    // Delete all endpoints
    zigbee_endpoint_t *ep = node->endpoints;
    while (ep != NULL) {
        zigbee_endpoint_t *next = ep->next;
        endpoint_delete(ep);
        ep = next;
    }
    
    if (node->manufacturer) {
        free(node->manufacturer);
    }
    if (node->model) {
        free(node->model);
    }
    
    free(node);
    TINY_LOG_D(TAG, "Deleted node");
}
