// Example: Timer subsystem usage

#include "timer.h"
#include "logger.h"
#include "event_bus.h"

static const char *TAG = "TIMER_EXAMPLE";

// Periodic timer callback - runs every second
void heartbeat_callback(void *arg)
{
    static uint32_t counter = 0;
    counter++;
    TINY_LOG_D(TAG, "Heartbeat: %lu", counter);
    
    // Could publish a heartbeat event
    event_bus_publish(EVENT_MQTT_CONNECTED, NULL, 0);
}

// One-shot timer callback - runs once after delay
void delayed_action_callback(void *arg)
{
    const char *action = (const char *)arg;
    TINY_LOG_I(TAG, "Executing delayed action: %s", action);
}

// Device timeout callback
void device_timeout_callback(void *arg)
{
    uint64_t device_addr = *(uint64_t *)arg;
    TINY_LOG_W(TAG, "Device timeout: 0x%016llx", device_addr);
    
    // Mark device as offline
    // Send notification
}

void setup_timers(void)
{
    // Create a periodic heartbeat timer (1 second interval)
    tiny_timer_t *heartbeat = timer_create(
        "heartbeat",
        1000000,  // 1 second in microseconds
        true,     // periodic
        heartbeat_callback,
        NULL
    );
    timer_start(heartbeat);
    
    // Create a one-shot delayed action timer (5 seconds)
    tiny_timer_t *delayed = timer_create(
        "delayed_action",
        5000000,  // 5 seconds
        false,    // one-shot
        delayed_action_callback,
        "Initialize Zigbee network"
    );
    timer_start(delayed);
    
    // Create a device watchdog timer (30 seconds)
    static uint64_t device_addr = 0x00124B001234ABCD;
    tiny_timer_t *watchdog = timer_create(
        "device_watchdog",
        30000000, // 30 seconds
        true,     // periodic
        device_timeout_callback,
        &device_addr
    );
    timer_start(watchdog);
}
