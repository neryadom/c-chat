#pragma once
#include <stdio.h>
#include <stdint.h>

typedef struct Parsed_Config_T {
    char ip[50];
    int ip_int;
    uint8_t ip_size;
    uint32_t port;
    uint32_t client_limit;
    uint32_t buffer_size;
    uint32_t idle_timeout;
} parsed_config_t;

void parse_line(char* line, char* lhs, char* rhs, char delimiter);

int32_t load_config(const char* filepath, parsed_config_t* server_config);
