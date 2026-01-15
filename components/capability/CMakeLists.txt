idf_component_register(
    SRCS 
        "capability.c"
        "light_capability.c"
        "switch_capability.c"
        "sensor_capability.c"
    INCLUDE_DIRS "include"
    REQUIRES 
        tiny_os
        zigbee_model
)
