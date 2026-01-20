<!--
Sync Impact Report:
- Version change: 0.0.0 → 1.0.0
- New constitution established from project specs
- Principles derived from: docs/spec/00_context_and_guardrails.yaml, docs/spec/project-plan.yaml
- Templates requiring updates: ✅ N/A (initial version)
- Follow-up TODOs: None
-->

# ESP32-C6 Zigbee Bridge OS Constitution

## Core Principles

### I. Single-Owner Domain Graph
Only the domain context may mutate the registry/device graph. All external ingress (Zigbee callbacks, MQTT messages, shell commands) MUST enqueue events to the bus and return immediately. Registry API functions MUST assert they are called from the domain context. This ensures deterministic state transitions and eliminates race conditions.

**Rationale**: Concurrent mutation of shared state leads to subtle bugs that are impossible to reproduce. A single owner with event-driven inputs makes the system debuggable and testable.

### II. Callbacks Enqueue Only
Zigbee and Wi-Fi callbacks MUST never run business logic; they enqueue events and return. Callback code size budget is <200 lines per module. No dynamic allocation in callback context. This keeps the vendor stack responsive and prevents stack overflow in constrained callback contexts.

**Rationale**: Vendor stacks (Zigbee, Wi-Fi) have strict timing and stack constraints. Blocking or allocating in callbacks causes crashes and undefined behavior.

### III. Deterministic Event Loop
Domain decisions are made by consuming bus events in a defined, repeatable order. The bus consumer is single-threaded. Event handlers MUST be short; schedule longer work via fibres. Events have typed payloads and correlation IDs for tracing.

**Rationale**: Determinism enables trace replay testing, reproducible bugs, and reasoning about system behavior without hardware.

### IV. Bounded Resources
Queues and buffers are fixed-size in v0; overflow policy is explicit. Each event type has a defined drop/coalesce policy. Drop counters MUST be visible via shell. No unbounded growth; the system fails gracefully under load.

**Rationale**: Embedded systems have hard memory limits. Explicit bounds and policies prevent silent failures and OOM crashes.

### V. Idempotent Outputs
Publishing (MQTT, HA discovery) MUST be idempotent for the same inputs. Use stable ordering and retain flags where appropriate. Re-publishing the same state MUST NOT create duplicate entities or spurious updates.

**Rationale**: Network unreliability means messages may be retried. Idempotency ensures the system converges to correct state regardless of delivery order.

### VI. Capability Abstraction
The capability layer provides stable, typed abstractions (light.on, sensor.temperature) decoupled from Zigbee cluster specifics. Northbound code (MQTT, HA discovery) operates on capabilities, never raw clusters. This preserves runway for Matter/HomeKit bridging.

**Rationale**: Zigbee internals are complex and device-specific. Capabilities provide a stable API that survives cluster quirks and enables multi-protocol bridging.

## Architecture Constraints

### Kernel-in-a-Task Model
The tiny OS runs inside FreeRTOS tasks. Vendor Zigbee/Wi-Fi stacks run in their required substrate tasks. Our OS schedules cooperative fibres for services and app logic, connected via the event bus. FreeRTOS is substrate only—not the application scheduling model.

### Node Lifecycle FSM
Devices follow a strict state machine: NEW → ANNOUNCED → INTERVIEWING → READY → OFFLINE → FAILED. EUI64 is the stable identity; NWK short address is telemetry only. State transitions emit events and trigger persistence.

### Persistence Policy
- Buffer updates; periodic flush every 5 seconds
- Immediate flush on lifecycle transitions (NEW→ANNOUNCED, INTERVIEWING→READY, etc.)
- Schema version key enables migration on firmware update
- Must persist: node identity, metadata, endpoints, clusters, capabilities, lifecycle state

## Development Standards

### Code Quality
- C11 with explicit sizes (uint32_t, not int)
- Module prefixes: os_, reg_, cap_, mqtt_, zb_
- Return explicit error codes; no silent failures
- Structured logging with levels (ERROR, WARN, INFO, DEBUG, TRACE)
- Safe to call LOG_* from any context (enqueues, never blocks)

### Testing
- Host simulation build for development without hardware
- Unit tests for all OS and service components
- Contract tests ensure fake adapters match real adapter behavior
- Trace replay enables deterministic integration testing

## Governance

This constitution supersedes all other development practices for this project. Amendments require:
1. Documented rationale for the change
2. Impact assessment on existing code
3. Migration plan if breaking existing behavior
4. Version increment following semantic versioning

All code reviews MUST verify compliance with these principles. Complexity MUST be justified against the principles. Use `docs/spec/` for detailed specifications and `STATUS.md` for project state tracking.

**Version**: 1.0.0 | **Ratified**: 2026-01-20 | **Last Amended**: 2026-01-20
