# ESP32-C6 Zigbee Bridge Examples

This directory contains example code demonstrating how to use the various components of the ESP32-C6 Zigbee Bridge.

## Examples

### 1. Device Control (`device_control.c`)
Demonstrates the complete data flow from Zigbee model to capability layer:
- Creating Zigbee nodes, endpoints, clusters, and attributes
- Using the capability layer for device control
- Handling device joined events
- Working with lights, switches, and sensors

### 2. Fibre Usage (`fibre_usage.c`)
Shows how to use fibres (green threads) for concurrent tasks:
- Creating fibres for different periodic tasks
- Cooperative multitasking with `fibre_yield()`
- Device polling, state synchronization, health monitoring

### 3. Timer Usage (`timer_usage.c`)
Demonstrates the timer subsystem:
- Periodic timers (e.g., heartbeat, watchdog)
- One-shot timers (e.g., delayed actions)
- Timer callbacks and event integration

### 4. Shell Commands (`shell_commands.c`)
Examples of custom shell commands:
- Listing paired devices
- Controlling lights via CLI
- Configuring MQTT settings
- Reading/writing persistent configuration

### 5. MQTT Integration (`mqtt_integration.c`)
MQTT bridge integration patterns:
- Publishing device states to MQTT
- Handling MQTT commands
- Event-driven state synchronization
- JSON payload construction

## Integration

These examples are standalone and not compiled by default. To use them in your application:

1. Copy the relevant code into your `main/main.c`
2. Call the setup functions from `app_main()`
3. Include the necessary headers

Example integration:
```c
#include "examples/device_control.c"
#include "examples/fibre_usage.c"
#include "examples/timer_usage.c"
#include "examples/shell_commands.c"
#include "examples/mqtt_integration.c"

void app_main(void)
{
    // ... existing initialization ...
    
    setup_event_handlers();
    setup_fibres();
    setup_timers();
    register_custom_commands();
    setup_mqtt_integration();
    
    // ... continue with normal operation ...
}
```

## Notes

- These examples are simplified for clarity
- Production code should include proper error handling
- JSON parsing should use a library like cJSON
- Device addresses and IDs are hardcoded for demonstration
