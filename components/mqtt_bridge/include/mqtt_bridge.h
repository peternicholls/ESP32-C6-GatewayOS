#ifndef MQTT_BRIDGE_H
#define MQTT_BRIDGE_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

// MQTT Bridge - Northbound interface for Zigbee devices
// Provides MQTT-first integration with runway for Matter/HomeKit bridging

/**
 * Initialize the MQTT bridge
 */
esp_err_t mqtt_bridge_init(void);

/**
 * Start the MQTT bridge
 */
esp_err_t mqtt_bridge_start(void);

/**
 * Stop the MQTT bridge
 */
esp_err_t mqtt_bridge_stop(void);

/**
 * Publish device state to MQTT
 */
esp_err_t mqtt_bridge_publish_state(uint64_t device_addr, const char *capability, const char *state);

/**
 * Check if MQTT is connected
 */
bool mqtt_bridge_is_connected(void);

#endif // MQTT_BRIDGE_H
