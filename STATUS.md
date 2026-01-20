# STATUS

## Current state
- Date (UTC): 2026-01-20
- Commit: fb9e98e-dirty
- Branch: main
- Last completed milestone: M8 (simulation), M1 (hardware verified)
- Next milestone in progress: M4 (Real Zigbee adapter)
- Active task IDs: T040 (Zigbee adapter integration)
- Known blockers: None - hardware now available

## Verified working
- [x] Boots reliably
- [x] Shell responsive over UART (output verified, input TBD)
- [x] Fibres switching (8 tasks running)
- [x] Event bus stable under load
- [x] Logging queue stable (no stalls)
- [x] Persistence stable across reboot
- [ ] Zigbee coordinator up (using fake adapter)
- [ ] Permit join works (requires real adapter)
- [x] Interview/provisioner works (simulated)
- [x] Registry persists and restores
- [ ] Light control end-to-end (requires real Zigbee)
- [ ] MQTT connects and stays connected (requires Wi-Fi)
- [x] MQTT publishes state updates (simulated)
- [ ] MQTT commands trigger Zigbee actions (requires real adapter)
- [x] HA Discovery service implemented
- [x] Device quirks system implemented
- [x] RGB LED blinking (addressable LED via led_strip driver)

## Progress notes
**Since last update**
- Hardware verified: ESP32-C6 dev board with ESP32-C6-WROOM-1 module
- FreeRTOS-based fibre scheduler working (replaced setjmp/longjmp)
- Addressable RGB LED on GPIO 8 working via led_strip driver
- All 8 fibres running: shell, blink, dispatch, interview, mqtt, ha_disc, local, idle
- Boot time ~200ms to shell prompt

**Today focus**
- M4: Implement real Zigbee adapter using ESP-IDF esp-zigbee-lib
- Wi-Fi connectivity for MQTT
- Verify shell input works

**Next actions**
- Create zb_real.c implementing zb_adapter.h contract
- Add Wi-Fi initialization and connection management
- Configure MQTT broker to real network address
- Test permit-join and device pairing

## Decisions locked in
- Substrate approach: ESP-IDF/FreeRTOS substrate; FreeRTOS tasks as fibres
- Capability taxonomy version: caps-v0
- Command semantics: actuators optimistic_with_reconcile; sensors report_driven
- MQTT topic scheme: bridge/<node_id>/<capability>/{state|set|meta}
- Persistence schema version: 1
- Queue overflow policy: drop_oldest_noncritical; log and count drops
- Node lifecycle FSM: NEW → ANNOUNCED → INTERVIEWING → READY → OFFLINE → FAILED
- LED: Addressable RGB on GPIO 8 via RMT/led_strip driver

## Metrics
- Unit tests: 38 passing (host simulation)
- OS components: 7 (fibre, event, log, console, shell, persist, types)
- Services: 6 (registry, interview, capability, reg_shell, ha_disc, quirks)
- Adapters: 2 (mqtt, zigbee fake)
- Fibres running: 8
- Binary size: 209KB (89% partition free)

## Open questions
- Shell input: Does UART rx work for interactive commands?
- Wi-Fi credentials: How to provision (hardcode initially, then shell command?)

## Blockers and risks
**Blockers**
- None

**Risks**
- R1: Zigbee stack callback context constraints (must enqueue only)
- R2: Wi-Fi/Zigbee coexistence on ESP32-C6
- R4: Cooperative scheduling starvation under heavy Zigbee traffic

## How to resume
Paste: Context Capsule + this STATUS.md + "Implement M4 (real Zigbee adapter) with ESP-IDF esp-zigbee-lib"
