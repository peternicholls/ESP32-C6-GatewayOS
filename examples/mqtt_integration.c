// Example: MQTT integration patterns

#include "mqtt_bridge.h"
#include "event_bus.h"
#include "logger.h"
#include "light_capability.h"
#include <string.h>
#include <stdio.h>

static const char *TAG = "MQTT_EXAMPLE";

// Handle MQTT command messages
void on_mqtt_message(event_type_t type, void *data, size_t data_len)
{
    // Parse topic: zigbee2mqtt/<device_addr>/set
    // Parse payload: {"state": "ON", "brightness": 128}
    
    // Example: Turn on a light based on MQTT command
    uint64_t device_addr = 0x00124B001234ABCD; // Parsed from topic
    
    light_device_t light = {
        .device_addr = device_addr,
        .endpoint_id = 1
    };
    
    // Parse JSON payload (simplified)
    // In production, use cJSON library
    light_on(&light);
    light_set_brightness(&light, 128);
    
    // Publish confirmation
    mqtt_bridge_publish_state(device_addr, "light", "{\"state\":\"ON\",\"brightness\":128}");
}

// Handle Zigbee attribute changes and publish to MQTT
void on_zigbee_attr_changed(event_type_t type, void *data, size_t data_len)
{
    // When a device attribute changes (e.g., light turned on physically),
    // publish the new state to MQTT
    
    uint64_t device_addr = 0x00124B001234ABCD;
    
    // Construct state JSON
    char state_json[256];
    snprintf(state_json, sizeof(state_json), 
             "{\"state\":\"ON\",\"brightness\":255,\"color_temp\":370}");
    
    // Publish to MQTT
    mqtt_bridge_publish_state(device_addr, "light", state_json);
    
    TINY_LOG_D(TAG, "Published state to MQTT for device 0x%016llx", device_addr);
}

// Setup MQTT integration
void setup_mqtt_integration(void)
{
    // Initialize MQTT bridge
    mqtt_bridge_init();
    
    // Subscribe to events
    event_bus_subscribe(EVENT_MQTT_MESSAGE_RECEIVED, on_mqtt_message);
    event_bus_subscribe(EVENT_ZIGBEE_ATTRIBUTE_CHANGED, on_zigbee_attr_changed);
    
    // Start MQTT client
    mqtt_bridge_start();
    
    TINY_LOG_I(TAG, "MQTT integration configured");
}

// Example: Publish various device states
void publish_device_states(void)
{
    // Light state
    mqtt_bridge_publish_state(
        0x00124B001234ABCD,
        "light",
        "{\"state\":\"ON\",\"brightness\":200,\"color_temp\":370}"
    );
    
    // Switch state
    mqtt_bridge_publish_state(
        0x00124B009876FEDC,
        "switch",
        "{\"state\":\"OFF\"}"
    );
    
    // Sensor readings
    mqtt_bridge_publish_state(
        0x00124B005555AAAA,
        "sensor",
        "{\"temperature\":22.5,\"humidity\":45,\"battery\":95}"
    );
}
