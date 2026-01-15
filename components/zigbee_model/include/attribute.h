#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

// Zigbee Attribute - data point within a cluster

typedef enum {
    ATTR_TYPE_BOOL,
    ATTR_TYPE_UINT8,
    ATTR_TYPE_UINT16,
    ATTR_TYPE_UINT32,
    ATTR_TYPE_INT8,
    ATTR_TYPE_INT16,
    ATTR_TYPE_INT32,
    ATTR_TYPE_STRING,
    ATTR_TYPE_ARRAY,
} attribute_type_t;

typedef union {
    bool bool_val;
    uint8_t u8_val;
    uint16_t u16_val;
    uint32_t u32_val;
    int8_t i8_val;
    int16_t i16_val;
    int32_t i32_val;
    char *string_val;
    void *array_val;
} attribute_value_t;

typedef struct zigbee_attribute {
    uint16_t attribute_id;
    attribute_type_t type;
    attribute_value_t value;
    bool reportable;
    struct zigbee_attribute *next;
} zigbee_attribute_t;

/**
 * Create a new attribute
 */
zigbee_attribute_t* attribute_create(uint16_t attribute_id, attribute_type_t type);

/**
 * Set attribute value
 */
esp_err_t attribute_set_value(zigbee_attribute_t *attr, attribute_value_t value);

/**
 * Get attribute value
 */
attribute_value_t attribute_get_value(zigbee_attribute_t *attr);

/**
 * Delete an attribute
 */
void attribute_delete(zigbee_attribute_t *attr);

#endif // ATTRIBUTE_H
