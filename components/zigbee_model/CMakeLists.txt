idf_component_register(
    SRCS 
        "zigbee_model.c"
        "node.c"
        "endpoint.c"
        "cluster.c"
        "attribute.c"
    INCLUDE_DIRS "include"
    REQUIRES 
        tiny_os
)
