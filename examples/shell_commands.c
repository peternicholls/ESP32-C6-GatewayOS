// Example: Shell command registration

#include "shell.h"
#include "logger.h"
#include "light_capability.h"
#include "persistence.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static const char *TAG = "SHELL_EXAMPLE";

// Custom command: List all paired devices
static esp_err_t cmd_devices(int argc, char **argv)
{
    printf("Paired Zigbee Devices:\n");
    printf("---------------------------------------------\n");
    printf("Addr             | Manufacturer | Model\n");
    printf("---------------------------------------------\n");
    printf("0x00124B001234AB | Philips      | Hue White\n");
    printf("0x00124B009876FE | IKEA         | TRADFRI\n");
    printf("---------------------------------------------\n");
    return ESP_OK;
}

// Custom command: Control a light
static esp_err_t cmd_light(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: light <addr> <on|off|brightness> [value]\n");
        printf("Examples:\n");
        printf("  light 0x1234 on\n");
        printf("  light 0x1234 off\n");
        printf("  light 0x1234 brightness 128\n");
        return ESP_ERR_INVALID_ARG;
    }
    
    uint64_t addr = strtoull(argv[1], NULL, 16);
    const char *action = argv[2];
    
    light_device_t light = {
        .device_addr = addr,
        .endpoint_id = 1
    };
    
    if (strcmp(action, "on") == 0) {
        light_on(&light);
        printf("Light 0x%016llx turned ON\n", addr);
    } else if (strcmp(action, "off") == 0) {
        light_off(&light);
        printf("Light 0x%016llx turned OFF\n", addr);
    } else if (strcmp(action, "brightness") == 0 && argc >= 4) {
        uint8_t brightness = atoi(argv[3]);
        light_set_brightness(&light, brightness);
        printf("Light 0x%016llx brightness set to %d\n", addr, brightness);
    } else {
        printf("Unknown action: %s\n", action);
        return ESP_ERR_INVALID_ARG;
    }
    
    return ESP_OK;
}

// Custom command: Configure MQTT
static esp_err_t cmd_mqtt_config(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: mqtt_config <broker> <port>\n");
        printf("Example: mqtt_config mqtt.example.com 1883\n");
        return ESP_ERR_INVALID_ARG;
    }
    
    const char *broker = argv[1];
    const char *port = argv[2];
    
    // Save to persistent storage
    persistence_set_str("mqtt", "broker", broker);
    persistence_set_str("mqtt", "port", port);
    
    printf("MQTT configured: %s:%s\n", broker, port);
    printf("Restart to apply changes\n");
    
    return ESP_OK;
}

// Custom command: Show configuration
static esp_err_t cmd_config(int argc, char **argv)
{
    char broker[128];
    char port[16];
    size_t len;
    
    printf("Current Configuration:\n");
    printf("---------------------\n");
    
    len = sizeof(broker);
    if (persistence_get_str("mqtt", "broker", broker, &len) == ESP_OK) {
        printf("MQTT Broker: %s\n", broker);
    } else {
        printf("MQTT Broker: Not configured\n");
    }
    
    len = sizeof(port);
    if (persistence_get_str("mqtt", "port", port, &len) == ESP_OK) {
        printf("MQTT Port: %s\n", port);
    } else {
        printf("MQTT Port: Not configured\n");
    }
    
    printf("---------------------\n");
    return ESP_OK;
}

// Register custom shell commands
void register_custom_commands(void)
{
    static const shell_cmd_t custom_commands[] = {
        {
            .name = "devices",
            .help = "List all paired Zigbee devices",
            .handler = cmd_devices
        },
        {
            .name = "light",
            .help = "Control a light device",
            .handler = cmd_light
        },
        {
            .name = "mqtt_config",
            .help = "Configure MQTT broker",
            .handler = cmd_mqtt_config
        },
        {
            .name = "config",
            .help = "Show current configuration",
            .handler = cmd_config
        },
    };
    
    for (size_t i = 0; i < sizeof(custom_commands) / sizeof(custom_commands[0]); i++) {
        shell_register_cmd(&custom_commands[i]);
    }
    
    TINY_LOG_I(TAG, "Registered %zu custom shell commands", 
               sizeof(custom_commands) / sizeof(custom_commands[0]));
}
