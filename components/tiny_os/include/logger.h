#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>
#include "esp_log.h"

// Enhanced logging framework for Tiny OS

typedef enum {
    LOG_LEVEL_NONE,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_VERBOSE
} log_level_t;

/**
 * Initialize the logger
 */
esp_err_t logger_init(void);

/**
 * Set log level for a module
 */
void logger_set_level(const char *module, log_level_t level);

/**
 * Log a message (wraps ESP_LOG macros with additional features)
 */
#define TINY_LOG_E(tag, format, ...) ESP_LOGE(tag, format, ##__VA_ARGS__)
#define TINY_LOG_W(tag, format, ...) ESP_LOGW(tag, format, ##__VA_ARGS__)
#define TINY_LOG_I(tag, format, ...) ESP_LOGI(tag, format, ##__VA_ARGS__)
#define TINY_LOG_D(tag, format, ...) ESP_LOGD(tag, format, ##__VA_ARGS__)
#define TINY_LOG_V(tag, format, ...) ESP_LOGV(tag, format, ##__VA_ARGS__)

#endif // LOGGER_H
