#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_netif.h"

#include "tiny_os.h"
#include "zigbee_model.h"
#include "capability.h"
#include "mqtt_bridge.h"

static const char *TAG = "MAIN";

void app_main(void)
{
    ESP_LOGI(TAG, "ESP32-C6 Zigbee Bridge starting...");
    
    // Initialize NVS for persistent storage
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    // Initialize network stack
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    // Initialize Wi-Fi
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    
    // Initialize Tiny OS kernel
    ESP_LOGI(TAG, "Initializing Tiny OS kernel...");
    tiny_os_init();
    
    // Initialize Zigbee model layer
    ESP_LOGI(TAG, "Initializing Zigbee model...");
    zigbee_model_init();
    
    // Initialize capability layer
    ESP_LOGI(TAG, "Initializing capability layer...");
    capability_init();
    
    // Initialize MQTT bridge
    ESP_LOGI(TAG, "Initializing MQTT bridge...");
    mqtt_bridge_init();
    
    ESP_LOGI(TAG, "ESP32-C6 Zigbee Bridge initialized successfully");
    
    // Start the Tiny OS kernel task
    tiny_os_start();
}
