/**
 * @file zb_real.c
 * @brief Real Zigbee adapter implementation using ESP-Zigbee stack
 *
 * ESP32-C6 Zigbee Bridge OS - Real Zigbee adapter (M4)
 *
 * Implements zb_adapter.h contract for ESP32-C6 hardware with:
 * - Coordinator network formation
 * - Device join/leave handling
 * - Event emission to OS bus
 */

#include "os_event.h"
#include "os_fibre.h"
#include "os_log.h"
#include "zb_adapter.h"

#include "esp_zigbee_core.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <string.h>

#define ZB_MODULE "ZB_REAL"

/* Configuration */
#define ZB_MAX_DEVICES 64
#define ZB_MAX_PENDING 16
#define ZB_TASK_STACK_SIZE 4096
#define ZB_TASK_PRIORITY 5
#define ZB_CMD_TIMEOUT_MS 5000

/* Forward declarations - internal helpers */
static void zb_task(void *arg);
static void zb_send_status_cb(esp_zb_zcl_command_send_status_message_t message);
static esp_err_t zb_core_action_cb(esp_zb_core_action_callback_id_t callback_id,
                                   const void *message);

/* ─────────────────────────────────────────────────────────────────────────────
 * State Machine
 * ─────────────────────────────────────────────────────────────────────────────
 */

typedef enum {
  ZB_STATE_UNINITIALIZED = 0,
  ZB_STATE_INITIALIZING,
  ZB_STATE_READY,
  ZB_STATE_ERROR,
} zb_state_t;

static zb_state_t s_zb_state = ZB_STATE_UNINITIALIZED;

/* State transition helper - T018 */
static const char *zb_state_name(zb_state_t state) {
  switch (state) {
  case ZB_STATE_UNINITIALIZED:
    return "UNINITIALIZED";
  case ZB_STATE_INITIALIZING:
    return "INITIALIZING";
  case ZB_STATE_READY:
    return "READY";
  case ZB_STATE_ERROR:
    return "ERROR";
  default:
    return "UNKNOWN";
  }
}

static bool zb_state_transition(zb_state_t new_state) {
  /* Validate allowed transitions */
  bool valid = false;
  switch (s_zb_state) {
  case ZB_STATE_UNINITIALIZED:
    valid = (new_state == ZB_STATE_INITIALIZING);
    break;
  case ZB_STATE_INITIALIZING:
    valid = (new_state == ZB_STATE_READY || new_state == ZB_STATE_ERROR);
    break;
  case ZB_STATE_READY:
    valid = (new_state == ZB_STATE_ERROR);
    break;
  case ZB_STATE_ERROR:
    valid = false; /* Terminal state */
    break;
  }
  if (!valid) {
    LOG_W(ZB_MODULE, "Invalid state transition: %s -> %s",
          zb_state_name(s_zb_state), zb_state_name(new_state));
    return false;
  }
  LOG_I(ZB_MODULE, "State: %s -> %s", zb_state_name(s_zb_state),
        zb_state_name(new_state));
  s_zb_state = new_state;
  return true;
}

/* ─────────────────────────────────────────────────────────────────────────────
 * Address Cache (EUI64 ↔ NWK mapping)
 * ─────────────────────────────────────────────────────────────────────────────
 */

typedef struct {
  os_eui64_t eui64;
  uint16_t nwk_addr;
  uint32_t last_seen_ms;
} zb_nwk_entry_t;

static zb_nwk_entry_t s_nwk_cache[ZB_MAX_DEVICES];
static uint8_t s_nwk_count = 0;

/* Address cache helpers - T007-T009 */
static zb_nwk_entry_t *nwk_cache_insert(os_eui64_t eui64, uint16_t nwk_addr) {
  /* First check if entry exists */
  for (uint8_t i = 0; i < s_nwk_count; i++) {
    if (s_nwk_cache[i].eui64 == eui64) {
      s_nwk_cache[i].nwk_addr = nwk_addr;
      s_nwk_cache[i].last_seen_ms =
          (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
      return &s_nwk_cache[i];
    }
  }
  /* Add new entry if space available */
  if (s_nwk_count >= ZB_MAX_DEVICES) {
    LOG_W(ZB_MODULE, "NWK cache full, cannot add " OS_EUI64_FMT,
          OS_EUI64_ARG(eui64));
    return NULL;
  }
  zb_nwk_entry_t *entry = &s_nwk_cache[s_nwk_count++];
  entry->eui64 = eui64;
  entry->nwk_addr = nwk_addr;
  entry->last_seen_ms = (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
  LOG_D(ZB_MODULE, "NWK cache add: " OS_EUI64_FMT " -> 0x%04X",
        OS_EUI64_ARG(eui64), nwk_addr);
  return entry;
}

static zb_nwk_entry_t *nwk_cache_lookup_by_eui64(os_eui64_t eui64) {
  for (uint8_t i = 0; i < s_nwk_count; i++) {
    if (s_nwk_cache[i].eui64 == eui64) {
      return &s_nwk_cache[i];
    }
  }
  return NULL;
}

static bool nwk_cache_remove(os_eui64_t eui64) {
  for (uint8_t i = 0; i < s_nwk_count; i++) {
    if (s_nwk_cache[i].eui64 == eui64) {
      /* Move last entry to this slot (swap-remove) */
      if (i < s_nwk_count - 1) {
        s_nwk_cache[i] = s_nwk_cache[s_nwk_count - 1];
      }
      s_nwk_count--;
      LOG_D(ZB_MODULE, "NWK cache remove: " OS_EUI64_FMT, OS_EUI64_ARG(eui64));
      return true;
    }
  }
  return false;
}

/* ─────────────────────────────────────────────────────────────────────────────
 * Correlation Tracking (command → response matching)
 * ─────────────────────────────────────────────────────────────────────────────
 */

typedef struct {
  uint8_t tsn;
  bool tsn_valid;
  os_corr_id_t corr_id;
  uint32_t timestamp_ms;
  bool in_use;
} zb_pending_cmd_t;

static zb_pending_cmd_t s_pending_cmds[ZB_MAX_PENDING];

/* Correlation tracking helpers - T012-T015 */
static zb_pending_cmd_t *pending_cmd_alloc(os_corr_id_t corr_id) {
  for (uint8_t i = 0; i < ZB_MAX_PENDING; i++) {
    if (!s_pending_cmds[i].in_use) {
      s_pending_cmds[i].corr_id = corr_id;
      s_pending_cmds[i].tsn = 0;
      s_pending_cmds[i].tsn_valid =
          false; /* Set true via pending_cmd_set_tsn */
      s_pending_cmds[i].timestamp_ms =
          (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
      s_pending_cmds[i].in_use = true;
      return &s_pending_cmds[i];
    }
  }
  LOG_W(ZB_MODULE, "Pending cmd slots full");
  return NULL;
}

static void pending_cmd_set_tsn(zb_pending_cmd_t *slot, uint8_t tsn) {
  if (slot && slot->in_use) {
    slot->tsn = tsn;
    slot->tsn_valid = true;
  }
}

static zb_pending_cmd_t *pending_cmd_lookup_by_tsn(uint8_t tsn) {
  for (uint8_t i = 0; i < ZB_MAX_PENDING; i++) {
    if (s_pending_cmds[i].in_use && s_pending_cmds[i].tsn_valid &&
        s_pending_cmds[i].tsn == tsn) {
      return &s_pending_cmds[i];
    }
  }
  return NULL;
}

static void pending_cmd_free(zb_pending_cmd_t *slot) {
  if (slot) {
    slot->in_use = false;
  }
}

/* ─────────────────────────────────────────────────────────────────────────────
 * Contract Functions - Lifecycle
 * ─────────────────────────────────────────────────────────────────────────────
 */

zb_err_t zb_init(void) {
  if (s_zb_state != ZB_STATE_UNINITIALIZED) {
    return OS_ERR_ALREADY_EXISTS;
  }

  LOG_I(ZB_MODULE, "Initializing Zigbee stack (real)");
  /* TODO: Implement in Phase 3 */
  return OS_ERR_NOT_READY;
}

zb_err_t zb_start_coordinator(void) {
  if (s_zb_state == ZB_STATE_UNINITIALIZED) {
    return OS_ERR_NOT_INITIALIZED;
  }
  if (s_zb_state == ZB_STATE_READY || s_zb_state == ZB_STATE_INITIALIZING) {
    return OS_OK;
  }
  return OS_ERR_NOT_READY;
}

zb_err_t zb_set_permit_join(uint16_t seconds) {
  if (s_zb_state != ZB_STATE_READY) {
    return OS_ERR_NOT_READY;
  }

  LOG_I(ZB_MODULE, "Permit join for %u seconds", seconds);
  /* TODO: Implement in Phase 4 */
  return OS_ERR_NOT_READY;
}

/* ─────────────────────────────────────────────────────────────────────────────
 * Contract Functions - Commands (implemented in zb_cmd.c)
 * ─────────────────────────────────────────────────────────────────────────────
 */

/* Note: zb_send_onoff, zb_send_level, zb_read_attrs, zb_configure_reporting,
 * zb_bind are implemented in zb_cmd.c */

/* ─────────────────────────────────────────────────────────────────────────────
 * Internal - Zigbee Task
 * ─────────────────────────────────────────────────────────────────────────────
 */

static void zb_task(void *arg) {
  (void)arg;
  LOG_I(ZB_MODULE, "Zigbee task starting");
  /* TODO: Implement in Phase 3 */
  vTaskDelete(NULL);
}

/* ─────────────────────────────────────────────────────────────────────────────
 * Internal - Callbacks (T019-T020)
 * ─────────────────────────────────────────────────────────────────────────────
 */

static void
zb_send_status_cb(esp_zb_zcl_command_send_status_message_t message) {
  LOG_D(ZB_MODULE, "send status cb called, tsn=%u status=%d", message.tsn,
        message.status);
  /* TODO: Implement in Phase 5 */
}

static esp_err_t zb_core_action_cb(esp_zb_core_action_callback_id_t callback_id,
                                   const void *message) {
  LOG_D(ZB_MODULE, "core action cb called: %d", callback_id);
  /* TODO: Implement in Phase 6 */
  return ESP_OK;
}

/* ─────────────────────────────────────────────────────────────────────────────
 * Signal Handler (called by ZBOSS stack)
 * ─────────────────────────────────────────────────────────────────────────────
 */

void esp_zb_app_signal_handler(esp_zb_app_signal_t *signal) {
  LOG_D(ZB_MODULE, "Signal handler called");
  /* TODO: Implement in Phase 3 */
}
