#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <stdint.h>
#include "esp_err.h"

// Event Bus - Publish/Subscribe messaging system

typedef enum {
    EVENT_ZIGBEE_DEVICE_JOINED,
    EVENT_ZIGBEE_DEVICE_LEFT,
    EVENT_ZIGBEE_ATTRIBUTE_CHANGED,
    EVENT_CAPABILITY_STATE_CHANGED,
    EVENT_MQTT_CONNECTED,
    EVENT_MQTT_DISCONNECTED,
    EVENT_MQTT_MESSAGE_RECEIVED,
    EVENT_WIFI_CONNECTED,
    EVENT_WIFI_DISCONNECTED,
    EVENT_MAX
} event_type_t;

typedef void (*event_handler_t)(event_type_t type, void *data, size_t data_len);

/**
 * Initialize the event bus
 */
esp_err_t event_bus_init(void);

/**
 * Subscribe to an event
 */
esp_err_t event_bus_subscribe(event_type_t type, event_handler_t handler);

/**
 * Publish an event
 */
esp_err_t event_bus_publish(event_type_t type, void *data, size_t data_len);

#endif // EVENT_BUS_H
