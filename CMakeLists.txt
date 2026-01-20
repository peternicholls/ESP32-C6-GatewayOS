# ESP32-C6 Zigbee Bridge OS - ESP-IDF Project
#
# This is the top-level CMakeLists.txt for the ESP-IDF build system.
# For host builds (testing), use the Makefile instead.

cmake_minimum_required(VERSION 3.16)

# Add extra component directories before including ESP-IDF
set(EXTRA_COMPONENT_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR}/os
    ${CMAKE_CURRENT_SOURCE_DIR}/services
    ${CMAKE_CURRENT_SOURCE_DIR}/adapters
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers
    ${CMAKE_CURRENT_SOURCE_DIR}/apps
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(esp32c6_zigbee_bridge)
