#include "attribute.h"
#include "logger.h"
#include <stdlib.h>
#include <string.h>

static const char *TAG = "ATTRIBUTE";

zigbee_attribute_t* attribute_create(uint16_t attribute_id, attribute_type_t type)
{
    zigbee_attribute_t *attr = malloc(sizeof(zigbee_attribute_t));
    if (!attr) {
        TINY_LOG_E(TAG, "Failed to allocate attribute");
        return NULL;
    }
    
    memset(attr, 0, sizeof(zigbee_attribute_t));
    attr->attribute_id = attribute_id;
    attr->type = type;
    attr->reportable = false;
    attr->next = NULL;
    
    TINY_LOG_D(TAG, "Created attribute: id=0x%04x, type=%d", attribute_id, type);
    
    return attr;
}

esp_err_t attribute_set_value(zigbee_attribute_t *attr, attribute_value_t value)
{
    if (!attr) {
        return ESP_ERR_INVALID_ARG;
    }
    
    // For string types, copy the string
    if (attr->type == ATTR_TYPE_STRING) {
        if (attr->value.string_val) {
            free(attr->value.string_val);
        }
        if (value.string_val) {
            attr->value.string_val = strdup(value.string_val);
        } else {
            attr->value.string_val = NULL;
        }
    } else {
        attr->value = value;
    }
    
    return ESP_OK;
}

attribute_value_t attribute_get_value(zigbee_attribute_t *attr)
{
    attribute_value_t empty = {0};
    
    if (!attr) {
        return empty;
    }
    
    return attr->value;
}

void attribute_delete(zigbee_attribute_t *attr)
{
    if (!attr) {
        return;
    }
    
    if (attr->type == ATTR_TYPE_STRING && attr->value.string_val) {
        free(attr->value.string_val);
    }
    
    free(attr);
    TINY_LOG_D(TAG, "Deleted attribute");
}
