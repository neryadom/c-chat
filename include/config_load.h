#pragma once
#include <stdio.h>
#include <stdint.h>

typedef struct Parsed_Config_T {
    char* ip;
    int ip_int;
    uint8_t ip_size;
    uint32_t port;
    uint32_t client_limit;
    uint32_t buffer_size;
    uint32_t idle_timeout;
} parsed_config_t;

parsed_config_t load_config(const char* filepath);
