/**
 * @file quirks.h
 * @brief Device quirks table API
 * 
 * ESP32-C6 Zigbee Bridge OS - Device quirks system
 * 
 * Provides escape hatch for non-standard Zigbee device behaviour.
 * Matches devices by manufacturer/model and applies corrective actions.
 */

#ifndef QUIRKS_H
#define QUIRKS_H

#include "os_types.h"
#include "capability.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Quirk action types */
typedef enum {
    QUIRK_ACTION_NONE = 0,
    QUIRK_ACTION_CLAMP_RANGE,       /* Clamp value to min/max range */
    QUIRK_ACTION_INVERT_BOOLEAN,    /* Invert true/false */
    QUIRK_ACTION_SCALE_NUMERIC,     /* Scale by multiplier/offset */
    QUIRK_ACTION_REMAP_ATTRIBUTE,   /* Map to different attribute */
    QUIRK_ACTION_OVERRIDE_REPORTING,/* Override reporting config */
    QUIRK_ACTION_IGNORE_SPURIOUS,   /* Ignore spurious reports */
} quirk_action_type_t;

/* Parameters for different action types */
typedef struct {
    int32_t min;
    int32_t max;
} quirk_clamp_params_t;

typedef struct {
    bool enabled;
} quirk_invert_params_t;

typedef struct {
    float multiplier;
    float offset;
} quirk_scale_params_t;

/* Union of all action parameters */
typedef union {
    quirk_clamp_params_t clamp;
    quirk_invert_params_t invert;
    quirk_scale_params_t scale;
} quirk_action_params_t;

/* Single quirk action */
typedef struct {
    quirk_action_type_t type;
    cap_id_t target_cap;
    quirk_action_params_t params;
} quirk_action_t;

/* Maximum actions per quirk entry */
#define QUIRK_MAX_ACTIONS 4

/* Quirk entry for a specific manufacturer/model */
typedef struct {
    const char *manufacturer;
    const char *model;
    bool prefix_match;              /* If true, match model prefix only */
    quirk_action_t actions[QUIRK_MAX_ACTIONS];
    uint8_t action_count;
} quirk_entry_t;

/* Result of applying quirks */
typedef struct {
    bool applied;
    uint8_t actions_applied;
} quirk_result_t;

/**
 * @brief Initialize quirks service
 * @return OS_OK on success
 */
os_err_t quirks_init(void);

/**
 * @brief Find quirk entry for a device
 * @param manufacturer Device manufacturer string
 * @param model Device model string
 * @return Pointer to quirk entry, or NULL if none
 */
const quirk_entry_t *quirks_find(const char *manufacturer, const char *model);

/**
 * @brief Apply quirks to a capability value
 * @param manufacturer Device manufacturer
 * @param model Device model
 * @param cap_id Capability ID
 * @param value In/out value pointer
 * @param result Output result (optional)
 * @return OS_OK on success
 */
os_err_t quirks_apply_value(const char *manufacturer, const char *model,
                             cap_id_t cap_id, cap_value_t *value,
                             quirk_result_t *result);

/**
 * @brief Apply quirks to a command value before encoding
 * @param manufacturer Device manufacturer
 * @param model Device model
 * @param cap_id Capability ID
 * @param value In/out value pointer
 * @param result Output result (optional)
 * @return OS_OK on success
 */
os_err_t quirks_apply_command(const char *manufacturer, const char *model,
                               cap_id_t cap_id, cap_value_t *value,
                               quirk_result_t *result);

/**
 * @brief Get the number of quirk entries in the table
 * @return Number of entries
 */
uint32_t quirks_count(void);

/**
 * @brief Get quirk entry by index
 * @param index Entry index
 * @return Pointer to entry, or NULL if invalid index
 */
const quirk_entry_t *quirks_get_entry(uint32_t index);

/**
 * @brief Get action type name
 * @param type Action type
 * @return Name string
 */
const char *quirks_action_name(quirk_action_type_t type);

#ifdef __cplusplus
}
#endif

#endif /* QUIRKS_H */
