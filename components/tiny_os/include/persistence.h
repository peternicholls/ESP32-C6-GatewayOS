#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

// Persistence layer using NVS (Non-Volatile Storage)

/**
 * Initialize persistence subsystem
 */
esp_err_t persistence_init(void);

/**
 * Store a value
 */
esp_err_t persistence_set_str(const char *namespace, const char *key, const char *value);
esp_err_t persistence_set_u8(const char *namespace, const char *key, uint8_t value);
esp_err_t persistence_set_u16(const char *namespace, const char *key, uint16_t value);
esp_err_t persistence_set_u32(const char *namespace, const char *key, uint32_t value);

/**
 * Retrieve a value
 */
esp_err_t persistence_get_str(const char *namespace, const char *key, char *value, size_t *len);
esp_err_t persistence_get_u8(const char *namespace, const char *key, uint8_t *value);
esp_err_t persistence_get_u16(const char *namespace, const char *key, uint16_t *value);
esp_err_t persistence_get_u32(const char *namespace, const char *key, uint32_t *value);

/**
 * Delete a value
 */
esp_err_t persistence_erase(const char *namespace, const char *key);

/**
 * Clear a namespace
 */
esp_err_t persistence_clear(const char *namespace);

#endif // PERSISTENCE_H
