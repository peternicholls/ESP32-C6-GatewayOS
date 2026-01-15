# ESP32-C6 Zigbee Bridge

A clean, modular Zigbee bridge implementation for ESP32-C6 with Wi-Fi + 802.15.4 (Zigbee) support.

## Architecture

This project implements a "tiny OS" kernel-in-a-task approach running on ESP-IDF/FreeRTOS, providing:

### Core Components

1. **Tiny OS Kernel** - Lightweight kernel-in-a-task substrate
   - **Fibres/Green Threads**: Cooperative multitasking scheduler
   - **Event Bus**: Publish/subscribe messaging system
   - **Timers**: High-resolution timer subsystem
   - **Logging**: Enhanced logging framework
   - **Persistence**: NVS-backed key-value storage
   - **Shell**: Interactive command-line interface

2. **Zigbee Model Layer** - Canonical Zigbee data model
   - **Node**: Represents a Zigbee device
   - **Endpoint**: Logical sub-device (application)
   - **Cluster**: Group of related attributes and commands
   - **Attribute**: Individual data point

3. **Capability Layer** - High-level device abstractions
   - **Light**: On/off, brightness, color control (maps to On/Off, Level Control, Color Control clusters)
   - **Switch**: Binary control (maps to On/Off cluster)
   - **Sensor**: Temperature, humidity, occupancy, battery (maps to measurement clusters)

4. **MQTT Bridge** - Northbound interface
   - MQTT-first design for cloud connectivity
   - Topic structure: `zigbee2mqtt/<device_addr>/<capability>/state`
   - Event-driven state publishing
   - Design runway for future Matter/HomeKit bridging

## Project Structure

```
ESP32-Zigbee-Bridge/
├── main/                      # Main application
│   ├── main.c                 # Application entry point
│   └── CMakeLists.txt
├── components/
│   ├── tiny_os/               # Tiny OS kernel
│   │   ├── include/           # Public headers
│   │   ├── tiny_os.c          # Kernel core
│   │   ├── fibre.c            # Green thread scheduler
│   │   ├── event_bus.c        # Event system
│   │   ├── timer.c            # Timer subsystem
│   │   ├── logger.c           # Logging framework
│   │   ├── persistence.c      # NVS storage
│   │   └── shell.c            # Interactive shell
│   ├── zigbee_model/          # Zigbee canonical model
│   │   ├── include/
│   │   ├── zigbee_model.c
│   │   ├── node.c             # Node abstraction
│   │   ├── endpoint.c         # Endpoint management
│   │   ├── cluster.c          # Cluster implementation
│   │   └── attribute.c        # Attribute handling
│   ├── capability/            # Device capability layer
│   │   ├── include/
│   │   ├── capability.c
│   │   ├── light_capability.c # Light control
│   │   ├── switch_capability.c# Switch control
│   │   └── sensor_capability.c# Sensor reading
│   └── mqtt_bridge/           # MQTT northbound interface
│       ├── include/
│       └── mqtt_bridge.c
├── CMakeLists.txt             # Top-level build config
├── sdkconfig.defaults         # ESP-IDF configuration
└── partitions.csv             # Partition table

## Building

### Prerequisites

- ESP-IDF v5.1 or later
- ESP32-C6 development board

### Setup

1. Install ESP-IDF:
```bash
# Follow instructions at https://docs.espressif.com/projects/esp-idf/en/latest/get-started/
```

2. Set up the environment:
```bash
. $HOME/esp/esp-idf/export.sh
```

3. Configure the project:
```bash
idf.py menuconfig
```

4. Build:
```bash
idf.py build
```

5. Flash:
```bash
idf.py -p /dev/ttyUSB0 flash monitor
```

## Usage

### Interactive Shell

The device provides an interactive shell over UART:

```
zbbridge> help
Available commands:
  help            - Show available commands
  version         - Show version information
  status          - Show system status

zbbridge> status
System Status:
  Free heap: 245632 bytes
  Min free heap: 242816 bytes
```

### MQTT Integration

Connect to an MQTT broker (configured in `mqtt_bridge.c`):

- **State topics**: `zigbee2mqtt/<device_addr>/<capability>/state`
- **Command topics**: `zigbee2mqtt/<device_addr>/set`

Example state message:
```json
{
  "state": "ON",
  "brightness": 255
}
```

## Design Principles

1. **Layered Architecture**: Clear separation between hardware abstraction, Zigbee model, capabilities, and northbound interfaces
2. **Vendor Agnostic**: Capability layer provides stable abstractions independent of vendor-specific Zigbee implementations
3. **Event-Driven**: Event bus decouples components and enables reactive programming
4. **MQTT-First**: Primary northbound interface designed for cloud and home automation platforms
5. **Future-Ready**: Architecture designed to support Matter and HomeKit bridging alongside MQTT

## Future Enhancements

- [ ] Zigbee coordinator implementation using ESP-Zigbee SDK
- [ ] Device pairing and commissioning flow
- [ ] Persistent device database
- [ ] OTA firmware updates
- [ ] Matter protocol bridge
- [ ] HomeKit accessory bridge
- [ ] Web UI for device management
- [ ] Network configuration via BLE provisioning

## License

See LICENSE file for details.
