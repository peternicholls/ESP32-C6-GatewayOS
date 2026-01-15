#include "fibre.h"
#include "logger.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

static const char *TAG = "FIBRE";

static fibre_t *fibre_list = NULL;
static fibre_t *current_fibre = NULL;

esp_err_t fibre_init(void)
{
    TINY_LOG_I(TAG, "Fibre scheduler initialized");
    return ESP_OK;
}

fibre_t* fibre_create(const char *name, fibre_func_t func, void *arg, uint32_t stack_size)
{
    fibre_t *fibre = malloc(sizeof(fibre_t));
    if (!fibre) {
        TINY_LOG_E(TAG, "Failed to allocate fibre");
        return NULL;
    }
    
    fibre->name = name;
    fibre->func = func;
    fibre->arg = arg;
    fibre->stack_size = stack_size;
    fibre->running = false;
    fibre->next = NULL;
    
    // Add to list
    if (fibre_list == NULL) {
        fibre_list = fibre;
    } else {
        fibre_t *last = fibre_list;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = fibre;
    }
    
    TINY_LOG_D(TAG, "Created fibre: %s", name);
    return fibre;
}

void fibre_yield(void)
{
    // In this simple implementation, yield just returns
    // A full implementation would save context and switch to next fibre
    taskYIELD();
}

void fibre_schedule(void)
{
    fibre_t *fibre = fibre_list;
    
    while (fibre != NULL) {
        if (!fibre->running) {
            current_fibre = fibre;
            fibre->running = true;
            
            // Execute the fibre function
            fibre->func(fibre->arg);
            
            fibre->running = false;
            current_fibre = NULL;
        }
        fibre = fibre->next;
    }
}
