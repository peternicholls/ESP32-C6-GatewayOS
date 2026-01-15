#ifndef TINY_OS_H
#define TINY_OS_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

// Tiny OS - A lightweight kernel-in-a-task for ESP32
// Provides green threads (fibres), event bus, timers, logging, persistence, and shell

/**
 * Initialize the Tiny OS kernel
 */
esp_err_t tiny_os_init(void);

/**
 * Start the Tiny OS kernel task
 */
esp_err_t tiny_os_start(void);

/**
 * Get the Tiny OS kernel status
 */
bool tiny_os_is_running(void);

#endif // TINY_OS_H
