// Example: Creating and managing a Zigbee light device
// This demonstrates the complete data flow from Zigbee model to capability layer

#include "zigbee_model.h"
#include "node.h"
#include "endpoint.h"
#include "cluster.h"
#include "attribute.h"
#include "light_capability.h"
#include "event_bus.h"

// Example: Device joined event handler
void on_device_joined(event_type_t type, void *data, size_t data_len)
{
    // Extract device information from event data
    uint64_t ieee_addr = 0x00124B001234ABCD;
    uint16_t short_addr = 0x1234;
    
    // Create a new node
    zigbee_node_t *node = node_create(short_addr, ieee_addr);
    node->manufacturer = "Philips";
    node->model = "Hue White Bulb";
    
    // Create endpoint 1 (main light endpoint)
    zigbee_endpoint_t *endpoint = endpoint_create(1, 0x0104, 0x0100); // HA profile, Dimmable Light
    
    // Add On/Off cluster
    zigbee_cluster_t *on_off_cluster = cluster_create(CLUSTER_ID_ON_OFF, true);
    zigbee_attribute_t *on_off_attr = attribute_create(0x0000, ATTR_TYPE_BOOL);
    attribute_value_t val = {.bool_val = false};
    attribute_set_value(on_off_attr, val);
    cluster_add_attribute(on_off_cluster, on_off_attr);
    endpoint_add_cluster(endpoint, on_off_cluster);
    
    // Add Level Control cluster
    zigbee_cluster_t *level_cluster = cluster_create(CLUSTER_ID_LEVEL_CONTROL, true);
    zigbee_attribute_t *level_attr = attribute_create(0x0000, ATTR_TYPE_UINT8);
    val.u8_val = 255;
    attribute_set_value(level_attr, val);
    cluster_add_attribute(level_cluster, level_attr);
    endpoint_add_cluster(endpoint, level_cluster);
    
    // Add endpoint to node
    node_add_endpoint(node, endpoint);
    node_set_online(node, true);
    
    // Now we can control the light using the capability layer
    light_device_t light = {
        .device_addr = ieee_addr,
        .endpoint_id = 1
    };
    
    // Turn the light on
    light_on(&light);
    
    // Set brightness to 50%
    light_set_brightness(&light, 128);
}

// Example: Attribute changed event handler
void on_attribute_changed(event_type_t type, void *data, size_t data_len)
{
    // When a Zigbee attribute changes (e.g., light turned on via physical switch),
    // this handler would:
    // 1. Update the local attribute value in the Zigbee model
    // 2. Trigger capability layer state change
    // 3. Publish state to MQTT via event bus
    
    // This creates a reactive system where state flows automatically:
    // Zigbee Network -> Zigbee Model -> Capability -> MQTT
}

// Example: Using the capability layer to control devices
void control_devices_example(void)
{
    // Light control
    light_device_t bedroom_light = {
        .device_addr = 0x00124B001234ABCD,
        .endpoint_id = 1
    };
    
    light_on(&bedroom_light);
    light_set_brightness(&bedroom_light, 200);
    light_set_color_temp(&bedroom_light, 370); // Warm white
    
    // Switch control
    switch_device_t living_room_switch = {
        .device_addr = 0x00124B009876FEDC,
        .endpoint_id = 1
    };
    
    switch_toggle(&living_room_switch);
    
    // Sensor reading
    sensor_device_t temp_sensor = {
        .device_addr = 0x00124B005555AAAA,
        .endpoint_id = 1
    };
    
    float temperature;
    if (sensor_get_temperature(&temp_sensor, &temperature) == ESP_OK) {
        printf("Temperature: %.1f°C\n", temperature);
    }
}

// Example: Registering event handlers
void setup_event_handlers(void)
{
    event_bus_subscribe(EVENT_ZIGBEE_DEVICE_JOINED, on_device_joined);
    event_bus_subscribe(EVENT_ZIGBEE_ATTRIBUTE_CHANGED, on_attribute_changed);
}
