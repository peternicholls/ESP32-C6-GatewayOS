#include "mqtt_bridge.h"
#include "logger.h"
#include "event_bus.h"
#include "mqtt_client.h"
#include <string.h>

static const char *TAG = "MQTT_BRIDGE";

// MQTT configuration (would be loaded from NVS in production)
#define MQTT_BROKER_URI "mqtt://localhost:1883"
#define MQTT_BASE_TOPIC "zigbee2mqtt"

static esp_mqtt_client_handle_t mqtt_client = NULL;
static bool mqtt_connected = false;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, 
                               int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;
    
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            TINY_LOG_I(TAG, "MQTT connected");
            mqtt_connected = true;
            event_bus_publish(EVENT_MQTT_CONNECTED, NULL, 0);
            
            // Subscribe to command topics
            char topic[128];
            snprintf(topic, sizeof(topic), "%s/+/set", MQTT_BASE_TOPIC);
            esp_mqtt_client_subscribe(mqtt_client, topic, 0);
            break;
            
        case MQTT_EVENT_DISCONNECTED:
            TINY_LOG_I(TAG, "MQTT disconnected");
            mqtt_connected = false;
            event_bus_publish(EVENT_MQTT_DISCONNECTED, NULL, 0);
            break;
            
        case MQTT_EVENT_SUBSCRIBED:
            TINY_LOG_D(TAG, "MQTT subscribed, msg_id=%d", event->msg_id);
            break;
            
        case MQTT_EVENT_DATA:
            TINY_LOG_D(TAG, "MQTT data received: topic=%.*s, data=%.*s",
                      event->topic_len, event->topic,
                      event->data_len, event->data);
            event_bus_publish(EVENT_MQTT_MESSAGE_RECEIVED, event, sizeof(*event));
            break;
            
        case MQTT_EVENT_ERROR:
            TINY_LOG_E(TAG, "MQTT error");
            break;
            
        default:
            break;
    }
}

// Event handler for device joined
static void on_device_joined(event_type_t type, void *data, size_t data_len)
{
    TINY_LOG_I(TAG, "Device joined event received");
    // Would publish device info to MQTT discovery topic
}

// Event handler for device left
static void on_device_left(event_type_t type, void *data, size_t data_len)
{
    TINY_LOG_I(TAG, "Device left event received");
    // Would publish device unavailable to MQTT
}

// Event handler for attribute changed
static void on_attribute_changed(event_type_t type, void *data, size_t data_len)
{
    TINY_LOG_D(TAG, "Attribute changed event received");
    // Would publish state update to MQTT
}

esp_err_t mqtt_bridge_init(void)
{
    TINY_LOG_I(TAG, "MQTT bridge initialized");
    TINY_LOG_I(TAG, "Northbound: MQTT-first with runway for Matter/HomeKit");
    
    // Subscribe to Zigbee events with proper handlers
    event_bus_subscribe(EVENT_ZIGBEE_DEVICE_JOINED, on_device_joined);
    event_bus_subscribe(EVENT_ZIGBEE_DEVICE_LEFT, on_device_left);
    event_bus_subscribe(EVENT_ZIGBEE_ATTRIBUTE_CHANGED, on_attribute_changed);
    
    return ESP_OK;
}

esp_err_t mqtt_bridge_start(void)
{
    const esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_BROKER_URI,
    };
    
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    if (mqtt_client == NULL) {
        TINY_LOG_E(TAG, "Failed to initialize MQTT client");
        return ESP_FAIL;
    }
    
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    
    esp_err_t ret = esp_mqtt_client_start(mqtt_client);
    if (ret != ESP_OK) {
        TINY_LOG_E(TAG, "Failed to start MQTT client");
        return ret;
    }
    
    TINY_LOG_I(TAG, "MQTT bridge started");
    return ESP_OK;
}

esp_err_t mqtt_bridge_stop(void)
{
    if (mqtt_client) {
        esp_mqtt_client_stop(mqtt_client);
        esp_mqtt_client_destroy(mqtt_client);
        mqtt_client = NULL;
    }
    
    mqtt_connected = false;
    TINY_LOG_I(TAG, "MQTT bridge stopped");
    return ESP_OK;
}

esp_err_t mqtt_bridge_publish_state(uint64_t device_addr, const char *capability, const char *state)
{
    if (!mqtt_connected || !mqtt_client) {
        return ESP_ERR_INVALID_STATE;
    }
    
    char topic[128];
    snprintf(topic, sizeof(topic), "%s/%016llx/%s/state", MQTT_BASE_TOPIC, device_addr, capability);
    
    int msg_id = esp_mqtt_client_publish(mqtt_client, topic, state, strlen(state), 0, 0);
    if (msg_id < 0) {
        TINY_LOG_E(TAG, "Failed to publish message");
        return ESP_FAIL;
    }
    
    TINY_LOG_D(TAG, "Published: %s = %s", topic, state);
    return ESP_OK;
}

bool mqtt_bridge_is_connected(void)
{
    return mqtt_connected;
}
