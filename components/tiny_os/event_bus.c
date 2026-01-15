#include "event_bus.h"
#include "logger.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include <string.h>

static const char *TAG = "EVENT_BUS";

#define MAX_HANDLERS_PER_EVENT 10

typedef struct {
    event_handler_t handlers[MAX_HANDLERS_PER_EVENT];
    uint32_t handler_count;
} event_handlers_t;

static event_handlers_t event_handlers[EVENT_MAX];
static SemaphoreHandle_t event_bus_mutex = NULL;

esp_err_t event_bus_init(void)
{
    memset(event_handlers, 0, sizeof(event_handlers));
    
    event_bus_mutex = xSemaphoreCreateMutex();
    if (event_bus_mutex == NULL) {
        TINY_LOG_E(TAG, "Failed to create mutex");
        return ESP_FAIL;
    }
    
    TINY_LOG_I(TAG, "Event bus initialized");
    return ESP_OK;
}

esp_err_t event_bus_subscribe(event_type_t type, event_handler_t handler)
{
    if (type >= EVENT_MAX) {
        return ESP_ERR_INVALID_ARG;
    }
    
    xSemaphoreTake(event_bus_mutex, portMAX_DELAY);
    
    if (event_handlers[type].handler_count >= MAX_HANDLERS_PER_EVENT) {
        xSemaphoreGive(event_bus_mutex);
        TINY_LOG_E(TAG, "Max handlers reached for event %d", type);
        return ESP_ERR_NO_MEM;
    }
    
    event_handlers[type].handlers[event_handlers[type].handler_count++] = handler;
    
    xSemaphoreGive(event_bus_mutex);
    
    TINY_LOG_D(TAG, "Subscribed to event %d", type);
    return ESP_OK;
}

esp_err_t event_bus_publish(event_type_t type, void *data, size_t data_len)
{
    if (type >= EVENT_MAX) {
        return ESP_ERR_INVALID_ARG;
    }
    
    xSemaphoreTake(event_bus_mutex, portMAX_DELAY);
    
    uint32_t count = event_handlers[type].handler_count;
    event_handler_t handlers[MAX_HANDLERS_PER_EVENT];
    memcpy(handlers, event_handlers[type].handlers, sizeof(event_handler_t) * count);
    
    xSemaphoreGive(event_bus_mutex);
    
    // Call handlers outside of mutex to avoid deadlocks
    for (uint32_t i = 0; i < count; i++) {
        handlers[i](type, data, data_len);
    }
    
    return ESP_OK;
}
