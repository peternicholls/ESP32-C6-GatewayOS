#ifndef FIBRE_H
#define FIBRE_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

// Fibre - Green threads / cooperative multitasking

typedef void (*fibre_func_t)(void *arg);

typedef struct fibre {
    fibre_func_t func;
    void *arg;
    uint32_t stack_size;
    const char *name;
    bool running;
    struct fibre *next;
} fibre_t;

/**
 * Initialize the fibre scheduler
 */
esp_err_t fibre_init(void);

/**
 * Create a new fibre
 */
fibre_t* fibre_create(const char *name, fibre_func_t func, void *arg, uint32_t stack_size);

/**
 * Yield execution to the next fibre
 */
void fibre_yield(void);

/**
 * Run the fibre scheduler
 */
void fibre_schedule(void);

#endif // FIBRE_H
