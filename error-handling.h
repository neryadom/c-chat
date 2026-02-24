#pragma once
#include <stdint.h>

void error_get_sockfd(int32_t sockfd);

void error_handler(int32_t ret, char* stage);