/**
 * @file zb_cmd.c
 * @brief Zigbee command sending implementation
 *
 * ESP32-C6 Zigbee Bridge OS - Zigbee command functions (M4)
 *
 * Implements command functions from zb_adapter.h:
 * - zb_send_onoff
 * - zb_send_level
 * - zb_read_attrs
 * - zb_configure_reporting
 * - zb_bind
 */

#include "os_log.h"
#include "zb_adapter.h"

#include "esp_zigbee_core.h"

#define ZB_MODULE "ZB_CMD"

/* ─────────────────────────────────────────────────────────────────────────────
 * External declarations from zb_real.c
 * ─────────────────────────────────────────────────────────────────────────────
 */

/* These will be exposed via internal header or extern in Phase 5 */

/* ─────────────────────────────────────────────────────────────────────────────
 * Command Functions
 * ─────────────────────────────────────────────────────────────────────────────
 */

zb_err_t zb_send_onoff(zb_node_id_t node_id, uint8_t endpoint, bool on,
                       os_corr_id_t corr_id) {
  (void)node_id;
  (void)endpoint;
  (void)on;
  (void)corr_id;

  LOG_I(ZB_MODULE, "zb_send_onoff (not implemented)");
  /* TODO: Implement in Phase 5 */
  return OS_ERR_NOT_READY;
}

zb_err_t zb_send_level(zb_node_id_t node_id, uint8_t endpoint,
                       uint8_t level_0_100, uint16_t transition_ms,
                       os_corr_id_t corr_id) {
  (void)node_id;
  (void)endpoint;
  (void)level_0_100;
  (void)transition_ms;
  (void)corr_id;

  LOG_I(ZB_MODULE, "zb_send_level (not implemented)");
  /* TODO: Implement in Phase 5 */
  return OS_ERR_NOT_READY;
}

zb_err_t zb_read_attrs(zb_node_id_t node_id, uint8_t endpoint,
                       uint16_t cluster_id, const uint16_t *attr_ids,
                       size_t attr_count, os_corr_id_t corr_id) {
  (void)node_id;
  (void)endpoint;
  (void)cluster_id;
  (void)attr_ids;
  (void)attr_count;
  (void)corr_id;

  LOG_I(ZB_MODULE, "zb_read_attrs (not implemented)");
  /* TODO: Implement in Phase 6 */
  return OS_ERR_NOT_READY;
}

zb_err_t zb_configure_reporting(zb_node_id_t node_id, uint8_t endpoint,
                                uint16_t cluster_id, uint16_t attr_id,
                                uint16_t min_s, uint16_t max_s,
                                uint32_t change) {
  (void)node_id;
  (void)endpoint;
  (void)cluster_id;
  (void)attr_id;
  (void)min_s;
  (void)max_s;
  (void)change;

  LOG_I(ZB_MODULE, "zb_configure_reporting (not implemented)");
  /* TODO: Implement in Phase 6 */
  return OS_ERR_NOT_READY;
}

zb_err_t zb_bind(zb_node_id_t node_id, uint8_t endpoint, uint16_t cluster_id,
                 uint64_t dst) {
  (void)node_id;
  (void)endpoint;
  (void)cluster_id;
  (void)dst;

  LOG_I(ZB_MODULE, "zb_bind (not implemented)");
  /* TODO: Implement in Phase 6 */
  return OS_ERR_NOT_READY;
}
