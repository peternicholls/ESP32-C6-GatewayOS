#include "shell.h"
#include "logger.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include <string.h>

static const char *TAG = "SHELL";

#define MAX_COMMANDS 50
#define MAX_CMD_LINE 256

static shell_cmd_t registered_commands[MAX_COMMANDS];
static uint32_t command_count = 0;

// Built-in commands
static esp_err_t cmd_help(int argc, char **argv)
{
    printf("\nAvailable commands:\n");
    for (uint32_t i = 0; i < command_count; i++) {
        printf("  %-15s - %s\n", registered_commands[i].name, registered_commands[i].help);
    }
    printf("\n");
    return ESP_OK;
}

static esp_err_t cmd_version(int argc, char **argv)
{
    printf("ESP32-C6 Zigbee Bridge v1.0.0\n");
    printf("Tiny OS kernel-in-a-task\n");
    return ESP_OK;
}

static esp_err_t cmd_status(int argc, char **argv)
{
    printf("System Status:\n");
    printf("  Free heap: %lu bytes\n", esp_get_free_heap_size());
    printf("  Min free heap: %lu bytes\n", esp_get_minimum_free_heap_size());
    return ESP_OK;
}

static const shell_cmd_t builtin_commands[] = {
    { "help", "Show available commands", cmd_help },
    { "version", "Show version information", cmd_version },
    { "status", "Show system status", cmd_status },
};

esp_err_t shell_init(void)
{
    // Register built-in commands
    for (size_t i = 0; i < sizeof(builtin_commands) / sizeof(builtin_commands[0]); i++) {
        shell_register_cmd(&builtin_commands[i]);
    }
    
    TINY_LOG_I(TAG, "Shell initialized with %lu commands", command_count);
    return ESP_OK;
}

esp_err_t shell_register_cmd(const shell_cmd_t *cmd)
{
    if (command_count >= MAX_COMMANDS) {
        TINY_LOG_E(TAG, "Maximum commands reached");
        return ESP_ERR_NO_MEM;
    }
    
    registered_commands[command_count++] = *cmd;
    return ESP_OK;
}

static void shell_task(void *arg)
{
    char cmd_line[MAX_CMD_LINE];
    
    printf("\n");
    printf("================================================\n");
    printf("  ESP32-C6 Zigbee Bridge Shell\n");
    printf("  Type 'help' for available commands\n");
    printf("================================================\n");
    printf("\n");
    
    while (1) {
        printf("zbbridge> ");
        fflush(stdout);
        
        if (fgets(cmd_line, sizeof(cmd_line), stdin) == NULL) {
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }
        
        // Remove newline
        cmd_line[strcspn(cmd_line, "\n")] = 0;
        
        if (strlen(cmd_line) == 0) {
            continue;
        }
        
        // Parse command
        char *argv[10];
        int argc = 0;
        char *token = strtok(cmd_line, " ");
        
        while (token != NULL && argc < 10) {
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }
        
        if (argc == 0) {
            continue;
        }
        
        // Find and execute command
        bool found = false;
        for (uint32_t i = 0; i < command_count; i++) {
            if (strcmp(argv[0], registered_commands[i].name) == 0) {
                registered_commands[i].handler(argc, argv);
                found = true;
                break;
            }
        }
        
        if (!found) {
            printf("Unknown command: %s\n", argv[0]);
            printf("Type 'help' for available commands\n");
        }
    }
}

esp_err_t shell_start(void)
{
    BaseType_t ret = xTaskCreate(
        shell_task,
        "shell",
        4096,
        NULL,
        2,
        NULL
    );
    
    if (ret != pdPASS) {
        TINY_LOG_E(TAG, "Failed to create shell task");
        return ESP_FAIL;
    }
    
    TINY_LOG_I(TAG, "Shell task started");
    return ESP_OK;
}
