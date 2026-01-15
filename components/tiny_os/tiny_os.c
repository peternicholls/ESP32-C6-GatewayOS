#include "tiny_os.h"
#include "fibre.h"
#include "event_bus.h"
#include "timer.h"
#include "logger.h"
#include "persistence.h"
#include "shell.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "TINY_OS";
static bool os_initialized = false;
static bool os_running = false;

#define TINY_OS_TASK_STACK_SIZE 8192
#define TINY_OS_TASK_PRIORITY 5

static void tiny_os_task(void *arg)
{
    TINY_LOG_I(TAG, "Tiny OS kernel task started");
    os_running = true;
    
    while (1) {
        // Run fibre scheduler
        fibre_schedule();
        
        // Small delay to allow other tasks to run
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

esp_err_t tiny_os_init(void)
{
    if (os_initialized) {
        return ESP_OK;
    }
    
    TINY_LOG_I(TAG, "Initializing Tiny OS kernel...");
    
    // Initialize subsystems
    ESP_ERROR_CHECK(logger_init());
    ESP_ERROR_CHECK(fibre_init());
    ESP_ERROR_CHECK(event_bus_init());
    ESP_ERROR_CHECK(timer_subsystem_init());
    ESP_ERROR_CHECK(persistence_init());
    ESP_ERROR_CHECK(shell_init());
    
    os_initialized = true;
    TINY_LOG_I(TAG, "Tiny OS kernel initialized");
    
    return ESP_OK;
}

esp_err_t tiny_os_start(void)
{
    if (!os_initialized) {
        return ESP_ERR_INVALID_STATE;
    }
    
    if (os_running) {
        return ESP_OK;
    }
    
    TINY_LOG_I(TAG, "Starting Tiny OS kernel task...");
    
    BaseType_t ret = xTaskCreate(
        tiny_os_task,
        "tiny_os",
        TINY_OS_TASK_STACK_SIZE,
        NULL,
        TINY_OS_TASK_PRIORITY,
        NULL
    );
    
    if (ret != pdPASS) {
        TINY_LOG_E(TAG, "Failed to create Tiny OS task");
        return ESP_FAIL;
    }
    
    // Start the shell
    shell_start();
    
    return ESP_OK;
}

bool tiny_os_is_running(void)
{
    return os_running;
}
