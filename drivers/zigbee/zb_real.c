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

/* ─────────────────────────────────────────────────────────────────────────────
 * Correlation Tracking (command → response matching)
 * ─────────────────────────────────────────────────────────────────────────────
 */

typedef struct {
  uint8_t tsn;
  os_corr_id_t corr_id;
  uint32_t timestamp_ms;
  bool in_use;
} zb_pending_cmd_t;

static zb_pending_cmd_t s_pending_cmds[ZB_MAX_PENDING];

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
 * Internal - Callbacks
 * ─────────────────────────────────────────────────────────────────────────────
 */

static void
zb_send_status_cb(esp_zb_zcl_command_send_status_message_t message) {
  LOG_D(ZB_MODULE, "Send status callback called");
  /* TODO: Implement in Phase 5 */
}

static esp_err_t zb_core_action_cb(esp_zb_core_action_callback_id_t callback_id,
                                   const void *message) {
  LOG_D(ZB_MODULE, "Core action callback called: %d", callback_id);
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
