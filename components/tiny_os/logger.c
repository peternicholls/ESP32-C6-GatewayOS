#include "logger.h"
#include "esp_log.h"

static const char *TAG = "LOGGER";

esp_err_t logger_init(void)
{
    // Set default log levels
    esp_log_level_set("*", ESP_LOG_INFO);
    ESP_LOGI(TAG, "Logger initialized");
    return ESP_OK;
}

void logger_set_level(const char *module, log_level_t level)
{
    esp_log_level_t esp_level;
    
    switch (level) {
        case LOG_LEVEL_NONE:
            esp_level = ESP_LOG_NONE;
            break;
        case LOG_LEVEL_ERROR:
            esp_level = ESP_LOG_ERROR;
            break;
        case LOG_LEVEL_WARN:
            esp_level = ESP_LOG_WARN;
            break;
        case LOG_LEVEL_INFO:
            esp_level = ESP_LOG_INFO;
            break;
        case LOG_LEVEL_DEBUG:
            esp_level = ESP_LOG_DEBUG;
            break;
        case LOG_LEVEL_VERBOSE:
            esp_level = ESP_LOG_VERBOSE;
            break;
        default:
            esp_level = ESP_LOG_INFO;
    }
    
    esp_log_level_set(module, esp_level);
}
