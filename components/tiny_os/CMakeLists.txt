idf_component_register(
    SRCS 
        "tiny_os.c"
        "fibre.c"
        "event_bus.c"
        "timer.c"
        "logger.c"
        "persistence.c"
        "shell.c"
    INCLUDE_DIRS "include"
    REQUIRES 
        freertos
        nvs_flash
        esp_timer
)
