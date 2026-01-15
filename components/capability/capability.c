#include "capability.h"
#include "logger.h"

static const char *TAG = "CAPABILITY";

esp_err_t capability_init(void)
{
    TINY_LOG_I(TAG, "Capability layer initialized");
    TINY_LOG_I(TAG, "Stable abstractions: light, switch, sensor");
    TINY_LOG_I(TAG, "Maps to Zigbee clusters for vendor-agnostic control");
    return ESP_OK;
}
