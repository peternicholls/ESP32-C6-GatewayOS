idf_component_register(
    SRCS 
        "mqtt_bridge.c"
    INCLUDE_DIRS "include"
    REQUIRES 
        tiny_os
        zigbee_model
        capability
        mqtt
)
