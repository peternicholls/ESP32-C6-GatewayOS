#include "timer.h"
#include "logger.h"
#include "esp_timer.h"
#include <string.h>
#include <stdlib.h>

static const char *TAG = "TIMER";

static void timer_callback_wrapper(void *arg)
{
    tiny_timer_t *timer = (tiny_timer_t *)arg;
    if (timer && timer->callback) {
        timer->callback(timer->arg);
    }
}

esp_err_t timer_subsystem_init(void)
{
    TINY_LOG_I(TAG, "Timer subsystem initialized");
    return ESP_OK;
}

tiny_timer_t* timer_create(const char *name, uint64_t period_us, bool periodic,
                           timer_callback_t callback, void *arg)
{
    tiny_timer_t *timer = malloc(sizeof(tiny_timer_t));
    if (!timer) {
        TINY_LOG_E(TAG, "Failed to allocate timer");
        return NULL;
    }
    
    timer->name = name;
    timer->period_us = period_us;
    timer->periodic = periodic;
    timer->callback = callback;
    timer->arg = arg;
    timer->active = false;
    timer->esp_timer_handle = NULL;
    
    TINY_LOG_D(TAG, "Created timer: %s", name);
    return timer;
}

esp_err_t timer_start(tiny_timer_t *timer)
{
    if (!timer) {
        return ESP_ERR_INVALID_ARG;
    }
    
    // Stop and delete existing timer if active
    if (timer->esp_timer_handle != NULL) {
        esp_timer_stop((esp_timer_handle_t)timer->esp_timer_handle);
        esp_timer_delete((esp_timer_handle_t)timer->esp_timer_handle);
        timer->esp_timer_handle = NULL;
    }
    
    const esp_timer_create_args_t timer_args = {
        .callback = timer_callback_wrapper,
        .arg = timer,
        .name = timer->name
    };
    
    esp_timer_handle_t esp_timer;
    esp_err_t err = esp_timer_create(&timer_args, &esp_timer);
    if (err != ESP_OK) {
        TINY_LOG_E(TAG, "Failed to create ESP timer");
        return err;
    }
    
    timer->esp_timer_handle = (void *)esp_timer;
    
    if (timer->periodic) {
        err = esp_timer_start_periodic(esp_timer, timer->period_us);
    } else {
        err = esp_timer_start_once(esp_timer, timer->period_us);
    }
    
    if (err != ESP_OK) {
        esp_timer_delete(esp_timer);
        timer->esp_timer_handle = NULL;
        TINY_LOG_E(TAG, "Failed to start ESP timer");
        return err;
    }
    
    timer->active = true;
    TINY_LOG_D(TAG, "Started timer: %s", timer->name);
    
    return ESP_OK;
}

esp_err_t timer_stop(tiny_timer_t *timer)
{
    if (!timer) {
        return ESP_ERR_INVALID_ARG;
    }
    
    if (timer->esp_timer_handle != NULL) {
        esp_err_t err = esp_timer_stop((esp_timer_handle_t)timer->esp_timer_handle);
        if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
            TINY_LOG_E(TAG, "Failed to stop ESP timer");
            return err;
        }
    }
    
    timer->active = false;
    TINY_LOG_D(TAG, "Stopped timer: %s", timer->name);
    
    return ESP_OK;
}

esp_err_t timer_delete(tiny_timer_t *timer)
{
    if (!timer) {
        return ESP_ERR_INVALID_ARG;
    }
    
    // Stop and delete ESP timer if it exists
    if (timer->esp_timer_handle != NULL) {
        esp_timer_stop((esp_timer_handle_t)timer->esp_timer_handle);
        esp_timer_delete((esp_timer_handle_t)timer->esp_timer_handle);
    }
    
    TINY_LOG_D(TAG, "Deleted timer: %s", timer->name);
    free(timer);
    
    return ESP_OK;
}
