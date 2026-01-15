#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>
#include "esp_err.h"

// Interactive shell for Tiny OS

typedef esp_err_t (*shell_cmd_handler_t)(int argc, char **argv);

typedef struct {
    const char *name;
    const char *help;
    shell_cmd_handler_t handler;
} shell_cmd_t;

/**
 * Initialize the shell
 */
esp_err_t shell_init(void);

/**
 * Register a command
 */
esp_err_t shell_register_cmd(const shell_cmd_t *cmd);

/**
 * Start the shell task
 */
esp_err_t shell_start(void);

#endif // SHELL_H
