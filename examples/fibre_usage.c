// Example: Using fibres (green threads) for concurrent tasks

#include "fibre.h"
#include "event_bus.h"
#include "logger.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"

static const char *TAG = "FIBRE_EXAMPLE";

// Fibre 1: Periodic device polling
void device_poll_fibre(void *arg)
{
    while (1) {
        TINY_LOG_D(TAG, "Polling devices...");
        
        // Poll devices for status updates
        // This is a cooperative task - it yields to allow other fibres to run
        
        fibre_yield();
        vTaskDelay(pdMS_TO_TICKS(5000)); // Poll every 5 seconds
    }
}

// Fibre 2: State synchronization
void state_sync_fibre(void *arg)
{
    while (1) {
        TINY_LOG_D(TAG, "Synchronizing state...");
        
        // Synchronize device states with persistent storage
        
        fibre_yield();
        vTaskDelay(pdMS_TO_TICKS(10000)); // Sync every 10 seconds
    }
}

// Fibre 3: Health monitoring
void health_monitor_fibre(void *arg)
{
    while (1) {
        TINY_LOG_D(TAG, "Checking system health...");
        
        // Check heap usage, connection status, etc.
        uint32_t free_heap = esp_get_free_heap_size();
        TINY_LOG_I(TAG, "Free heap: %lu bytes", free_heap);
        
        fibre_yield();
        vTaskDelay(pdMS_TO_TICKS(30000)); // Check every 30 seconds
    }
}

void setup_fibres(void)
{
    // Create fibres for different tasks
    // Each fibre runs cooperatively, yielding to others
    
    fibre_create("device_poll", device_poll_fibre, NULL, 4096);
    fibre_create("state_sync", state_sync_fibre, NULL, 4096);
    fibre_create("health_monitor", health_monitor_fibre, NULL, 4096);
    
    // The fibre scheduler in tiny_os.c will run these in round-robin fashion
}
