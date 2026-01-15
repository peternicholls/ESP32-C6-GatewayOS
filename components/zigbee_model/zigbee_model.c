#include "zigbee_model.h"
#include "node.h"
#include "endpoint.h"
#include "cluster.h"
#include "attribute.h"
#include "logger.h"

static const char *TAG = "ZIGBEE_MODEL";

static zigbee_node_t *node_list = NULL;

esp_err_t zigbee_model_init(void)
{
    TINY_LOG_I(TAG, "Zigbee model initialized");
    TINY_LOG_I(TAG, "Canonical model: Node -> Endpoint -> Cluster -> Attribute");
    return ESP_OK;
}
