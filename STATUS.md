# STATUS

## Current state
- Date (UTC): 2026-01-15
- Commit: (see git log)
- Branch: copilot/add-new-prompts-directory
- Last completed milestone: M8
- Next milestone in progress: M9 (Expand device classes)
- Active task IDs: T080 (completed)
- Known blockers: M4 (Zigbee adapter) requires ESP32 hardware

## Verified working
- [x] Boots reliably
- [x] Shell responsive over UART
- [x] Fibres switching (>=3 tasks)
- [x] Event bus stable under load
- [x] Logging queue stable (no stalls)
- [x] Persistence stable across reboot
- [ ] Zigbee coordinator up (requires ESP32 hardware)
- [ ] Permit join works (requires ESP32 hardware)
- [x] Interview/provisioner works (simulated)
- [x] Registry persists and restores
- [ ] Light control end-to-end (requires ESP32 hardware)
- [x] MQTT connects and stays connected (simulated)
- [x] MQTT publishes state updates (simulated)
- [ ] MQTT commands trigger Zigbee actions (requires ESP32 hardware)
- [x] HA Discovery service implemented
- [x] Device quirks system implemented

## Progress notes
**Since last update**
- M0: Created project structure and documentation
- M1: Implemented Tiny OS core (fibres, event bus, logging, console, shell)
- M2: Implemented persistence service (NVS wrapper with buffered writes)
- M3: Implemented device registry with lifecycle state machine
- M5: Implemented interview/provisioner service (simulated Zigbee)
- M6: Implemented capability mapping for lights (OnOff, Level clusters)
- M7: Implemented MQTT adapter (simulated for host testing)
- M8: Implemented HA Discovery service (T080) with:
  - Light merging (on/off + level -> single light entity)
  - Sensor discovery (temperature, humidity, contact, motion)
  - MQTT pending queue for offline handling
- Device quirks table system for non-standard devices
- Updated node lifecycle FSM states per 00_context_and_guardrails.yaml

**Today focus**
- Complete all implementable milestones without ESP32 hardware
- All 38 unit tests passing

**Next actions**
- M4: Zigbee adapter requires ESP32 hardware with ESP-IDF
- M9: Expand device classes
- M10: Matter bridge spike (bonus)

## Decisions locked in
- Substrate approach: ESP-IDF/FreeRTOS substrate; kernel-in-a-task for fibres/services
- Capability taxonomy version: caps-v0
- Command semantics: actuators optimistic_with_reconcile; sensors report_driven
- MQTT topic scheme: bridge/<node_id>/<capability>/{state|set|meta}
- Persistence schema version: 1
- Queue overflow policy: drop_oldest_noncritical; log and count drops
- Node lifecycle FSM: NEW → ANNOUNCED → INTERVIEWING → READY → OFFLINE → FAILED

## Metrics
- Unit tests: 38 passing
- OS components: 7 (fibre, event, log, console, shell, persist, types)
- Services: 6 (registry, interview, capability, reg_shell, ha_disc, quirks)
- Adapters: 1 (mqtt)

## Open questions
- None at this time

## Blockers and risks
**Blockers**
- M4 (Zigbee adapter) requires ESP32-C6 hardware with ESP-IDF

**Risks**
- R1: Zigbee stack callback context constraints
- R2: Persistence schema evolution
- R3: Device quirks / non-standard attributes (mitigated by quirks system)
- R4: Cooperative scheduling starvation

## How to resume
Paste: Context Capsule + this STATUS.md + the next task request (e.g., "Implement M4 with ESP32 hardware").
