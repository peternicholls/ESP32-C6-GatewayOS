#include "persistence.h"
#include "logger.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <string.h>

static const char *TAG = "PERSIST";

esp_err_t persistence_init(void)
{
    TINY_LOG_I(TAG, "Persistence subsystem initialized");
    return ESP_OK;
}

esp_err_t persistence_set_str(const char *namespace, const char *key, const char *value)
{
    nvs_handle_t handle;
    esp_err_t err;
    
    err = nvs_open(namespace, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        return err;
    }
    
    err = nvs_set_str(handle, key, value);
    if (err == ESP_OK) {
        err = nvs_commit(handle);
    }
    
    nvs_close(handle);
    return err;
}

esp_err_t persistence_set_u8(const char *namespace, const char *key, uint8_t value)
{
    nvs_handle_t handle;
    esp_err_t err;
    
    err = nvs_open(namespace, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        return err;
    }
    
    err = nvs_set_u8(handle, key, value);
    if (err == ESP_OK) {
        err = nvs_commit(handle);
    }
    
    nvs_close(handle);
    return err;
}

esp_err_t persistence_set_u16(const char *namespace, const char *key, uint16_t value)
{
    nvs_handle_t handle;
    esp_err_t err;
    
    err = nvs_open(namespace, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        return err;
    }
    
    err = nvs_set_u16(handle, key, value);
    if (err == ESP_OK) {
        err = nvs_commit(handle);
    }
    
    nvs_close(handle);
    return err;
}

esp_err_t persistence_set_u32(const char *namespace, const char *key, uint32_t value)
{
    nvs_handle_t handle;
    esp_err_t err;
    
    err = nvs_open(namespace, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        return err;
    }
    
    err = nvs_set_u32(handle, key, value);
    if (err == ESP_OK) {
        err = nvs_commit(handle);
    }
    
    nvs_close(handle);
    return err;
}

esp_err_t persistence_get_str(const char *namespace, const char *key, char *value, size_t *len)
{
    nvs_handle_t handle;
    esp_err_t err;
    
    err = nvs_open(namespace, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        return err;
    }
    
    err = nvs_get_str(handle, key, value, len);
    nvs_close(handle);
    
    return err;
}

esp_err_t persistence_get_u8(const char *namespace, const char *key, uint8_t *value)
{
    nvs_handle_t handle;
    esp_err_t err;
    
    err = nvs_open(namespace, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        return err;
    }
    
    err = nvs_get_u8(handle, key, value);
    nvs_close(handle);
    
    return err;
}

esp_err_t persistence_get_u16(const char *namespace, const char *key, uint16_t *value)
{
    nvs_handle_t handle;
    esp_err_t err;
    
    err = nvs_open(namespace, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        return err;
    }
    
    err = nvs_get_u16(handle, key, value);
    nvs_close(handle);
    
    return err;
}

esp_err_t persistence_get_u32(const char *namespace, const char *key, uint32_t *value)
{
    nvs_handle_t handle;
    esp_err_t err;
    
    err = nvs_open(namespace, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        return err;
    }
    
    err = nvs_get_u32(handle, key, value);
    nvs_close(handle);
    
    return err;
}

esp_err_t persistence_erase(const char *namespace, const char *key)
{
    nvs_handle_t handle;
    esp_err_t err;
    
    err = nvs_open(namespace, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        return err;
    }
    
    err = nvs_erase_key(handle, key);
    if (err == ESP_OK) {
        err = nvs_commit(handle);
    }
    
    nvs_close(handle);
    return err;
}

esp_err_t persistence_clear(const char *namespace)
{
    nvs_handle_t handle;
    esp_err_t err;
    
    err = nvs_open(namespace, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        return err;
    }
    
    err = nvs_erase_all(handle);
    if (err == ESP_OK) {
        err = nvs_commit(handle);
    }
    
    nvs_close(handle);
    return err;
}
