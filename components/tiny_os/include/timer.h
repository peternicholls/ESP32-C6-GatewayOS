#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

// Timer subsystem for the Tiny OS

typedef void (*timer_callback_t)(void *arg);

typedef struct tiny_timer {
    uint64_t period_us;
    timer_callback_t callback;
    void *arg;
    bool periodic;
    bool active;
    const char *name;
} tiny_timer_t;

/**
 * Initialize the timer subsystem
 */
esp_err_t timer_subsystem_init(void);

/**
 * Create a timer
 */
tiny_timer_t* timer_create(const char *name, uint64_t period_us, bool periodic, 
                           timer_callback_t callback, void *arg);

/**
 * Start a timer
 */
esp_err_t timer_start(tiny_timer_t *timer);

/**
 * Stop a timer
 */
esp_err_t timer_stop(tiny_timer_t *timer);

/**
 * Delete a timer
 */
esp_err_t timer_delete(tiny_timer_t *timer);

#endif // TIMER_H
