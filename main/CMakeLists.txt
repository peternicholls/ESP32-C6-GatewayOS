idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES 
        tiny_os 
        zigbee_model 
        capability 
        mqtt_bridge
        nvs_flash
        esp_wifi
        esp_netif
)
