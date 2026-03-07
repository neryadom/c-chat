#include "../../include/config_load.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

void parse_line(char* line, char* lhs, char* rhs, char delimiter) {
    size_t len_line = strlen(line);
    line[len_line - 1] = '\0'; /* take off the newline */
    int8_t limit = 0;
    for (int8_t i = 0; i < (int8_t) len_line; i++) {
        if (line[i] == delimiter) {
            limit = i;
            break;
        }
    }
    memcpy(lhs, line, limit);
    lhs[limit] = '\0';
    memcpy(rhs, line + limit + 1 /* after equal sign onwards */, (len_line - limit - 1));
    rhs[len_line - limit - 1] = '\0';
}


parsed_config_t load_config(const char* filepath){
    parsed_config_t server_config;
    FILE* f = fopen(filepath, "r");
    char line[100];

    while (fgets(line, 100, f) != NULL) {
        constexpr char char_delimiter = '=';
        char param_key[50] = {0};
        char param_value[50] = {0};
        parse_line(line, param_key, param_value, char_delimiter);

        if (strcmp(param_key, "ip") == 0) {
            strncpy(server_config.ip, param_value, sizeof(param_value));
            server_config.ip_size = strlen(param_value);
            inet_pton(AF_INET, server_config.ip, &server_config.ip_int);
            printf("Setting server ip to -> %s\n", server_config.ip);
            printf("Setting server ip_int (int repr) to -> %d\n", server_config.ip_int);
            printf("Setting server ip_size to -> %d\n", server_config.ip_size);
        } else if (strcmp(param_key, "port") == 0) {
            server_config.port = (int32_t) strtol(param_value, nullptr, 10);
            printf("Setting server port to -> %d\n", (int32_t) strtol(param_value, nullptr, 10));
        } else if (strcmp(param_key, "client_limit") == 0) {
            server_config.client_limit = strtol(param_value, nullptr, 10);
            printf("Setting server client limit to -> %d\n", server_config.client_limit);
        } else if (strcmp(param_key, "buffer_size") == 0) {
            server_config.buffer_size = strtol(param_value, nullptr, 10);
            printf("Setting server buffer size to -> %d\n", server_config.buffer_size);
        } else if (strcmp(param_key, "idle_timeout") == 0) {
            server_config.idle_timeout = strtol(param_value, nullptr, 10);
            printf("Setting server idle timeout to -> %d\n", server_config.idle_timeout);
        } else {
            printf("Unrecognized format, breaking...\n");
        }
    }
    return server_config;
}
